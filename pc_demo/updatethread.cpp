#include "updatethread.h"
#include "utils/utils.h"
#include <memory>
#include <QDebug>

namespace {

template<typename T>
void stopThread(T &t)
{
    if (t != nullptr) {
        t->requestInterruption();
        while (!t->wait(200))
            ;
        delete t;
        t = nullptr;
    }
}

uint16_t chip_crc16(const void *_ptr, uint32_t len)
{
    const uint8_t *ptr  = (const uint8_t *) _ptr;
    uint16_t crc_ta[16] = {
        0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
        0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
    };
    uint16_t crc;
    uint8_t da;

    crc = 0;
    while (len-- != 0) {
        da = crc >> 12;
        crc <<= 4;
        crc ^= crc_ta[da ^ (*ptr / 16)];

        da = crc >> 12;

        crc <<= 4;
        crc ^= crc_ta[da ^ (*ptr & 0x0f)];
        ptr++;
    }

    return (crc);
}

#define JL_SU_CMD_DEVICE_INIT 0xC0
#define JL_SU_CMD_DEVICE_CHECK 0xC1
#define JL_SU_CMD_ERASE 0xC2
#define JL_SU_CMD_WRITE 0xC3
#define JL_SU_CMD_FLASH_CRC 0xC4
#define JL_SU_CMD_EX_KEY 0xC5
#define JL_SU_CMD_REBOOT 0xCA

/* reply code */
#define JL_SU_RSP_SUCC 0x0
#define JL_SU_RSP_CRC_ERROR 0x1
#define JL_SU_RSP_SDK_ID_ERROR 0x2
#define JL_SU_RSP_OTHER_ERROR 0x3

#define JL_ERASE_TYPE_PAGE 1
#define JL_ERASE_TYPE_SECTOR 2
#define JL_ERASE_TYPE_BLOCK 3

#define JL_MAX_CRC_LIST_COUNT 16
#define JL_MAX_WRITE_SIZE 1024

#define TIMEOUT (8 * 1000) // 8s

std::shared_ptr<QByteArray> makeCommand(uint32_t encKey, uint8_t opcode,
                                        const std::vector<std::pair<const void *, uint32_t>> &bodys)
{
    uint32_t bodyLen = 0;
    for (auto &v : bodys) {
        bodyLen += v.second;
    }
    auto totalLen = 2 + 2 + 2 + bodyLen + 2;
    auto buf      = std::make_shared<QByteArray>(totalLen, '\0');
    auto p        = (uint8_t *) buf->data();
    p[0]          = 0xAA;
    p[1]          = 0x55;
    p[2]          = ((bodyLen + 2) >> 0) & 0xFF;
    p[3]          = ((bodyLen + 2) >> 8) & 0xFF;
    p[4]          = opcode;
    p[5]          = JL_SU_RSP_SUCC;

    auto bodyPtr = &p[6];
    for (auto &v : bodys) {
        memcpy(bodyPtr, v.first, v.second);
        bodyPtr += v.second;
    }

    // we encrypt now
    uint8_t *theKey = (uint8_t *) &encKey;
    uint32_t keyIdx = 0;
    for (unsigned i = 4; i < totalLen - 2; ++i, keyIdx++) {
        p[i] ^= theKey[keyIdx & 0x3];
    }

    auto crc                   = crc16(p, 2 + 2 + 2 + bodyLen);
    p[2 + 2 + 2 + bodyLen + 0] = (crc >> 0) & 0xFF;
    p[2 + 2 + 2 + bodyLen + 1] = (crc >> 8) & 0xFF;
    return buf;
}

std::shared_ptr<QByteArray> makeDeviceInitCmd(uint32_t encKey, const char *area, uint8_t mode)
{
    char buf[16 + 1];
    memset(buf, 0, sizeof(buf));
    strncpy(buf, area, 15);
    buf[16] = mode;
    return makeCommand(encKey, JL_SU_CMD_DEVICE_INIT, {{buf, sizeof(buf)}});
}

std::shared_ptr<QByteArray> makeDeviceCheckCmd(uint32_t encKey, uint32_t sdkId)
{
    return makeCommand(encKey, JL_SU_CMD_DEVICE_CHECK, {{&sdkId, 4}});
}

std::shared_ptr<QByteArray> makeEraseCmd(uint32_t encKey, uint32_t address, uint32_t type)
{
    return makeCommand(encKey, JL_SU_CMD_ERASE, {{&address, 4}, {&type, 4}});
}

std::shared_ptr<QByteArray> makeWriteCmd(uint32_t encKey, uint32_t address, uint32_t len,
                                         const void *content)
{
    return makeCommand(encKey, JL_SU_CMD_WRITE, {{&address, 4}, {&len, 4}, {content, len}});
}

std::shared_ptr<QByteArray> makeFlashCRCCmd(uint32_t encKey, uint32_t address, uint32_t len,
                                            uint32_t blockSize)
{
    return makeCommand(encKey, JL_SU_CMD_FLASH_CRC, {{&address, 4}, {&len, 4}, {&blockSize, 4}});
}

std::shared_ptr<QByteArray> makeRebootCmd(uint32_t encKey)
{
    return makeCommand(encKey, JL_SU_CMD_REBOOT, {});
}

bool isReplyDeviceInitCmd(const QByteArray &bin, uint8_t &rspStatus, uint32_t &upgrade_addr,
                          uint32_t &upgrade_len, uint32_t &upgrade_eoffset,
                          uint32_t &flash_alignsize)
{
    qDebug() << __func__ << bin.toHex() << ", size " << bin.size();
    if (bin.size() != 1 + 1 + 4 + 4 + 4 + 4) {
        return false;
    }
    if (uint8_t(bin[0]) != JL_SU_CMD_DEVICE_INIT || uint8_t(bin[1]) != JL_SU_RSP_SUCC) {
        rspStatus = uint8_t(bin[1]);
        return false;
    }
    memcpy(&upgrade_addr, bin.data() + 2, 4);
    memcpy(&upgrade_len, bin.data() + 2 + 4, 4);
    memcpy(&upgrade_eoffset, bin.data() + 2 + 4 + 4, 4);
    memcpy(&flash_alignsize, bin.data() + 2 + 4 + 4 + 4, 4);
    return true;
}

bool isReplyDeviceCheckCmd(const QByteArray &bin, uint8_t &rspStatus, uint8_t vid[4],
                           uint8_t pid[16], uint32_t &sdk_id)
{
    qDebug() << __func__ << bin.toHex() << ", size " << bin.size();
    if (bin.size() != 1 + 1 + 4 + 16 + 4) {
        return false;
    }
    if (uint8_t(bin[0]) != JL_SU_CMD_DEVICE_CHECK || uint8_t(bin[1]) != JL_SU_RSP_SUCC) {
        rspStatus = uint8_t(bin[1]);
        return false;
    }
    memcpy(&vid, bin.data() + 2, 4);
    memcpy(&pid, bin.data() + 2 + 4, 16);
    memcpy(&sdk_id, bin.data() + 2 + 4 + 16, 4);
    return true;
}

bool isReplyFlashCrcCmd(const QByteArray &bin, uint8_t &rspStatus, uint32_t blockCount,
                        uint16_t *crcList)
{
    qDebug() << __func__ << bin.toHex() << ", size " << bin.size();
    if (bin.size() != 1 + 1 + 2 * blockCount) {
        return false;
    }
    if (uint8_t(bin[0]) != JL_SU_CMD_FLASH_CRC || uint8_t(bin[1]) != JL_SU_RSP_SUCC) {
        rspStatus = uint8_t(bin[1]);
        return false;
    }
    memcpy(crcList, bin.data() + 2, blockCount * 2);
    return true;
}

bool isReplyEraseCmd(const QByteArray &bin, uint8_t &rspStatus)
{
    qDebug() << __func__ << bin.toHex() << ", size " << bin.size();
    if (bin.size() != 1 + 1) {
        return false;
    }
    if (uint8_t(bin[0]) != JL_SU_CMD_ERASE) {
        return false;
    }
    rspStatus = uint8_t(bin[1]);
    return true;
}

bool isReplyWriteCmd(const QByteArray &bin, uint8_t &rspStatus)
{
    qDebug() << __func__ << bin.toHex() << ", size " << bin.size();
    if (bin.size() != 1 + 1) {
        return false;
    }
    if (uint8_t(bin[0]) != JL_SU_CMD_WRITE) {
        return false;
    }
    rspStatus = uint8_t(bin[1]);
    return true;
}

} // namespace

UpdateThread::UpdateThread(const QString &portName, int updateBaudRate,
                           const std::shared_ptr<QByteArray> &fileContent, bool _isUpateUboot,
                           uint32_t encKey, QObject *parent)
    : QThread(parent), m_portName(portName), m_updateBaudRate(updateBaudRate),
      m_isUpdateUboot(_isUpateUboot), m_fileContent(fileContent), m_encKey(encKey)
{
    m_serialThread = new PhySerialThread(m_rawRemoteCmdQueue);
    connect(this, &UpdateThread::infoWriteBuffer, m_serialThread, &PhySerialThread::on_writeBuffer);
    connect(this, &UpdateThread::infoOpenRequest, m_serialThread, &PhySerialThread::on_openRequest);
    connect(this, &UpdateThread::infoReopenRequest, m_serialThread,
            &PhySerialThread::on_reOpenRequest);
    m_serialThread->start();
}

UpdateThread::~UpdateThread()
{
    stopThread(m_serialThread);
}

#define CHECK_TIMEOUT(cmd) \
    do { \
        if (!cmd) { \
            emit infoError(tr("timeout")); \
            return; \
        } \
    } while (0)

#define CHECK_TIMEOUT_RET(cmd) \
    do { \
        if (!cmd) { \
            emit infoError(tr("timeout")); \
            return false; \
        } \
    } while (0)

void UpdateThread::run()
{
    runInner();
    stopThread(m_serialThread);
}

std::shared_ptr<QByteArray> decodeCommand(const std::shared_ptr<QByteArray> &cmd, uint32_t encKey)
{
    if (cmd == nullptr) {
        return nullptr;
    }
    uint8_t *theKey = (uint8_t *) &encKey;
    unsigned keyIdx = 0;
    uint8_t *ptr    = (uint8_t *) cmd->data();
    for (int i = 0; i < cmd->size(); ++i, ++keyIdx) {
        ptr[i] ^= theKey[keyIdx & 0x3];
    }
    return cmd;
}

bool UpdateThread::deviceUpdateZoneInit(const char *name, uint8_t type, DeviceInitInfo &info)
{
    emit infoMessage(tr("send device init cmd: %1").arg(name));
    emit infoWriteBuffer(makeDeviceInitCmd(m_encKey, name, type), false);
    // and wait for reply
    auto cmd = waitCommand();
    if (cmd == nullptr) {
        emit infoError(tr("timeout"));
        return false;
    }

    uint32_t zoneAddr, upgradeLen, flashEoffsetSize, eraseUnitSize;
    uint8_t rspStatus;

    if (!isReplyDeviceInitCmd(*cmd, rspStatus, zoneAddr, upgradeLen, flashEoffsetSize,
                              eraseUnitSize)) {
        emit infoError(tr("[Error, invalid reply, rspStatus %1]").arg(uint32_t(rspStatus)));
        return false;
    }

    info.zoneAddr         = zoneAddr;
    info.upgradeLen       = upgradeLen;
    info.flashEoffsetSize = flashEoffsetSize;
    info.eraseUnitSize    = eraseUnitSize;
    return true;
}

std::unique_ptr<uint16_t[]> getBufferCrcList(const char *fileBuf, uint32_t blockCount,
                                             uint32_t eraseUnitSize)
{
    auto ubootFileCrcList = std::unique_ptr<uint16_t[]>(new uint16_t[blockCount]);

    for (uint32_t offset = 0; offset < blockCount; offset++) {
        ubootFileCrcList[offset] = chip_crc16(fileBuf + offset * eraseUnitSize, eraseUnitSize);
    }
    return ubootFileCrcList;
}

void UpdateThread::runInner()
{
    emit infoOpenRequest(m_portName, m_updateBaudRate);

    DeviceInitInfo app;

    if (!deviceUpdateZoneInit("app_dir_head", 0, app)) {
        return;
    }

    uint32_t appZoneAddr      = app.zoneAddr;
    uint32_t flashEoffsetSize = app.flashEoffsetSize;
    uint32_t eraseUnitSize    = app.eraseUnitSize;

    emit infoMessage(tr("send device check cmd"));
    // and perform device check
    emit infoWriteBuffer(makeDeviceCheckCmd(m_encKey, 0x12345678), false);
    auto cmd = waitCommand();
    CHECK_TIMEOUT(cmd);
    // parse
    uint8_t vid[4], pid[16];
    uint32_t replySdkId;
    uint8_t rspStatus;
    if (!isReplyDeviceCheckCmd(*cmd, rspStatus, vid, pid, replySdkId)) {
        emit infoError(tr("[Error, invalid reply, rspStatus %1]").arg(uint32_t(rspStatus)));
        return;
    }

    auto &fileContent = *m_fileContent;

    if (flashEoffsetSize == 4096) {
        qDebug() << "4K.bin";
        // jl_isd_4K.bin
    } else {
        qDebug() << "0K.bin";
        // jl_isd_0K.bin
    }

    if (unsigned(fileContent.size()) <= appZoneAddr) {
        return;
    }

    if (m_isUpdateUboot) {
        DeviceInitInfo uboot;

        // switch to zone
        if (!deviceUpdateZoneInit("uboot_zone", 0, uboot)) {
            return;
        }

        uint32_t backupUbootAddr = 65536; // 64K

        uint32_t abs64kLen    = backupUbootAddr - flashEoffsetSize;
        auto ubootFileContent = m_fileContent->mid(0, abs64kLen);
        if (ubootFileContent.size() < abs64kLen) {
            ubootFileContent.append(abs64kLen - ubootFileContent.size(), '\xFF');
        }

        // inform that we are updating
        if (!eraseFlash(appZoneAddr, eraseUnitSize)) {
            emit infoError(tr("failed to erase app zone"));
            return;
        }

        // write uboot to backup area (64K)
        if (!eraseFlash(backupUbootAddr, abs64kLen)) {
            emit infoError(tr("failed to erase 64K addr"));
            return;
        }
        uint32_t ubootBlockCount = abs64kLen / eraseUnitSize;
        auto ubootFileBuf        = ubootFileContent.data();

        if (!writeFlash(ubootFileBuf, backupUbootAddr, abs64kLen, eraseUnitSize)) {
            emit infoError(tr("failed to write flash"));
            return;
        }

        auto ubootFileCrcList = getBufferCrcList(ubootFileBuf, ubootBlockCount, eraseUnitSize);

        // check if crc match
        if (!ensureCrcMatch(ubootFileCrcList.get(), backupUbootAddr, ubootBlockCount,
                            eraseUnitSize)) {
            return;
        }

        // now, we should write to uboot area
        if (!eraseFlash(0 + flashEoffsetSize, abs64kLen)) {
            emit infoError(tr("failed to erase 0K addr"));
            return;
        }

        if (ubootBlockCount > 1) {
            if (!writeFlash(ubootFileBuf + 1 * eraseUnitSize, 1 * eraseUnitSize + flashEoffsetSize,
                            (ubootBlockCount - 1) * eraseUnitSize, eraseUnitSize)) {
                emit infoError(tr("failed to write flash"));
            }
        }

        if (!writeFlash(ubootFileBuf, 0 + flashEoffsetSize, eraseUnitSize, eraseUnitSize)) {
            emit infoError(tr("failed to write flash"));
            return;
        }

        if (!ensureCrcMatch(ubootFileCrcList.get(), 0 + flashEoffsetSize, ubootBlockCount,
                            eraseUnitSize)) {
            return;
        }

        // and switch back to appzone
        DeviceInitInfo _;
        if (!deviceUpdateZoneInit("app_dir_head", 0, _)) {
            emit infoError(tr("failed to switch to app zone"));
            return;
        }
    }

    uint32_t fileSize        = fileContent.size() - appZoneAddr;
    uint32_t blockCount      = (fileSize + eraseUnitSize - 1) / eraseUnitSize;
    uint32_t alignedFileSize = blockCount * eraseUnitSize;
    if (alignedFileSize != fileSize) {
        fileContent.append(alignedFileSize - fileSize, '\xFF');
    }

    auto chipCrcList = std::unique_ptr<uint16_t[]>(new uint16_t[blockCount]);

    auto fileBuf = fileContent.data() + appZoneAddr;

    auto fileCrcList = getBufferCrcList(fileBuf, blockCount, eraseUnitSize);

    // read crc in chip
    if (!fetchChipCrcList(0 + flashEoffsetSize, blockCount, eraseUnitSize, chipCrcList.get())) {
        return;
    }

    if (memcmp(fileCrcList.get(), chipCrcList.get(), 2 * blockCount) == 0) {
        // same crc, no need to upgrade
        emit infoMessage(tr("send reboot"));
        emit infoWriteBuffer(makeRebootCmd(m_encKey), true);
        waitCommand();
        return;
    }

    {
        auto upgradeAddr = 0 + flashEoffsetSize;
        if (!eraseFlash(upgradeAddr, eraseUnitSize)) {
            return;
        }
        for (uint32_t i = 1; i < blockCount; ++i) {
            uint32_t off = i * eraseUnitSize;
            if (fileCrcList[i] != chipCrcList[i]) {
                qDebug() << "should erase " << i << ", off = " << off
                         << " size = " << eraseUnitSize;
                if (!eraseFlash(upgradeAddr + off, eraseUnitSize)) {
                    return;
                }
                if (!writeFlash(fileBuf + off, upgradeAddr + off, eraseUnitSize, eraseUnitSize)) {
                    return;
                }
            }
        }
        // and finally write first unit
        if (!writeFlash(fileBuf + 0 * eraseUnitSize, upgradeAddr + 0 * eraseUnitSize, eraseUnitSize,
                        eraseUnitSize)) {
            return;
        }

        if (!ensureCrcMatch(fileCrcList.get(), 0 + flashEoffsetSize, blockCount, eraseUnitSize)) {
            emit infoError(tr("update failed, crc mismatch"));
            return;
        }

        // ask for reboot
        emit infoMessage(tr("update ok"));
        emit infoMessage(tr("send reboot"));
        emit infoWriteBuffer(makeRebootCmd(m_encKey), true);
        waitCommand(); // wait until we have write command
    }
}

bool UpdateThread::fetchChipCrcList(uint32_t address, uint32_t blockCount, uint32_t blockSize,
                                    uint16_t *crcList)
{
    while (blockCount > 0) {
        uint32_t readCount = blockCount > JL_MAX_CRC_LIST_COUNT ? JL_MAX_CRC_LIST_COUNT
                                                                : blockCount;
        emit infoWriteBuffer(makeFlashCRCCmd(m_encKey, address, readCount * blockSize, blockSize),
                             false);
        auto cmd = waitCommand();
        if (cmd == nullptr) {
            emit infoError(tr("timeout"));
            return false;
        }
        uint8_t rspStatus;
        // parse result
        if (!isReplyFlashCrcCmd(*cmd, rspStatus, readCount, crcList)) {
            emit infoError(tr("[Error, invalid reply, rspStatus %1]").arg(uint32_t(rspStatus)));
            return false;
        }

        address += readCount * blockSize;
        blockCount -= readCount;
        crcList += readCount;
    }
    return true;
}

bool UpdateThread::eraseFlash(uint32_t address, uint32_t len)
{
    if (len == 256) {
        return doEraseFlash(address, len);
    } else {
        while (len) {
            if (!doEraseFlash(address, 4096)) {
                return false;
            }
            address += 4096;
            len -= 4096;
        }
    }
    return true;
}

bool UpdateThread::doEraseFlash(uint32_t address, uint32_t len)
{
    uint8_t type = 0;
    if (len == 64 * 1024) {
        type = JL_ERASE_TYPE_BLOCK;
    } else if (len == 4096) {
        type = JL_ERASE_TYPE_SECTOR;
    } else if (len == 256) {
        type = JL_ERASE_TYPE_PAGE;
    }
    emit infoWriteBuffer(makeEraseCmd(m_encKey, address, type), false);
    auto cmd = waitCommand();
    if (cmd == nullptr) {
        emit infoError(tr("timeout"));
        return false;
    }
    uint8_t rspStatus;
    if (!isReplyEraseCmd(*cmd, rspStatus)) {
        return false;
    }
    if (rspStatus != JL_SU_RSP_SUCC) {
        emit infoError(tr("erase reply %1").arg(rspStatus));
        return false;
    }
    return true;
}

bool UpdateThread::writeFlash(const void *_buffer, uint32_t address, uint32_t len,
                              uint32_t flash_erase_unit)
{
    const uint8_t *buffer = (const uint8_t *) _buffer;
    while (len > 0) {
        uint32_t writeLen = len > flash_erase_unit ? flash_erase_unit : len;
        if (!doWriteFlash(buffer, address, writeLen)) {
            return false;
        }
        address += writeLen;
        buffer += writeLen;
        len -= writeLen;
        qDebug() << "after write flash: len = " << len << ", writeLen = " << writeLen;
    }
    return true;
}

bool UpdateThread::doWriteFlash(const void *buffer, uint32_t address, uint32_t len)
{
    emit infoWriteBuffer(makeWriteCmd(m_encKey, address, len, buffer), false);
    auto cmd = waitCommand();
    if (cmd == nullptr) {
        emit infoError(tr("timeout"));
        return false;
    }
    uint8_t rspStatus;
    if (!isReplyWriteCmd(*cmd, rspStatus)) {
        emit infoError(tr("invalid reply"));
        return false;
    }
    if (rspStatus != JL_SU_RSP_SUCC) {
        emit infoError(tr("write reply %1").arg(rspStatus));
        return false;
    }
    return true;
}

bool UpdateThread::ensureCrcMatch(const uint16_t *fileCrcList, uint32_t addr, uint32_t blockCount,
                                  uint32_t eraseUnitSize)
{
    auto ubootChipCrcList = std::unique_ptr<uint16_t[]>(new uint16_t[blockCount]);
    // check if we write to backup area successfully
    if (!fetchChipCrcList(addr, blockCount, eraseUnitSize, ubootChipCrcList.get())) {
        return false;
    }

    if (memcmp(ubootChipCrcList.get(), fileCrcList, 2 * blockCount) != 0) {
        emit infoError(("after uboot write 64K, crc mismatch"));
        return false;
    }
    return true;
}

std::shared_ptr<QByteArray> UpdateThread::waitCommand()
{
    std::shared_ptr<QByteArray> cmd;
    if (!m_rawRemoteCmdQueue.poll(cmd, TIMEOUT)) {
        // timeout-ed
        return nullptr;
    }
    return decodeCommand(cmd, m_encKey);
}
