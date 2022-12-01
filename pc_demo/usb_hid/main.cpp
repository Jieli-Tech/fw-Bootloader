#include <QCoreApplication>
#include "hidapi.h"
#include <QDebug>
#include <QThread>
#include <QFile>

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



uint16_t crc16(const void *_ptr, uint32_t len)
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

uint16_t *getBufferCrcList(const char *fileBuf, uint32_t blockCount,
                           uint32_t eraseUnitSize)
{
    auto ubootFileCrcList = new uint16_t[blockCount];

    for (uint32_t offset = 0; offset < blockCount; offset++) {
        ubootFileCrcList[offset] = crc16(fileBuf + offset * eraseUnitSize, eraseUnitSize);
    }
    return ubootFileCrcList;
}

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define SYNC_MARK0 0xAA
#define SYNC_MARK1 0x55



//上位机结构体
typedef struct {
    u8 syncdata0;
    u8 syncdata1;
    u16 cmd_len;
    u8 cmd;
    u8 rsp_status;
    union {
        struct {
            u32 sdk_id;
        } __attribute__((packed)) device_check;

        struct {
            u32 address;		// 烧写FLASH地址
            u32 data_length;    //烧写长度
            u8  data[0];		// 烧写数据
        } __attribute__((packed)) write;

        struct {
            u32 address;    //需要对齐
            u32 type;
        } __attribute__((packed)) erase;

        struct {
            u32 address;
            u32 len;
            u32 block_size;
        } __attribute__((packed)) crc_list;

        struct {
            char file_name[16];
            u8 mode;
        } __attribute__((packed))  init;
    };
} __attribute__((packed)) JL_SECTOR_COMMAND_ITEM;

//设备结构体
typedef struct {
    u8 syncdata0;
    u8 syncdata1;
    u16 cmd_len;
    u8 cmd;
    u8 rsp_status;
    union {
        struct {
            u16 crc[0];
        } __attribute__((packed)) crc_list;

        struct {
            u32 upgrade_addr;
            u32 upgrade_len;
            u32 upgrade_eoffset;
            u32 flash_alignsize;
        } __attribute__((packed))  init;

        struct {
            u8 vid[4];
            u8 pid[16];
            u32 sdk_id;
        } __attribute__((packed)) device_check;
    };
} __attribute__((packed)) JL_SECTOR_COMMAND_DEV_ITEM;

struct DeviceInitInfo {
    uint32_t zoneAddr, upgradeLen, flashEoffsetSize, eraseUnitSize;
};

JL_SECTOR_COMMAND_DEV_ITEM dev_cmd_ops;
hid_device *handle;



//10进制，注意要与上位机一致，不然无法升级
uint32_t communication_key = 12345678;

void decodeCommand(uint8_t *buf, uint32_t len)
{
    uint32_t keyIdx = 0;
    uint8_t *encKey = (uint8_t *)&communication_key;

    for (uint32_t i = 0; i < len; ++i, ++keyIdx) {
        buf[i] ^= encKey[keyIdx & 0x3];
    }
}
static u8 hid_rx_buf[65];
int read_deice(JL_SECTOR_COMMAND_DEV_ITEM *dev_cmd_ops)
{
    dev_cmd_ops->rsp_status = 0xff;
    memset(dev_cmd_ops, 0xff, sizeof(JL_SECTOR_COMMAND_DEV_ITEM));
    u32 timeout = 3000;
    while (timeout--) {
        int r = hid_read(handle, hid_rx_buf, 65);
        if (r >= 64) {
            decodeCommand(hid_rx_buf + 4, 60);
            memcpy(dev_cmd_ops, hid_rx_buf, sizeof(JL_SECTOR_COMMAND_DEV_ITEM));
            //  qDebug()<<"rsp_len"<<dev_cmd_ops->cmd_len;
            break;
        }

        QThread::msleep(1);
    }
    return 0;
}

static u8 hid_tx_buf[65];

void makeCommand(uint8_t opcode, const std::vector<std::pair<const void *, uint32_t>> &bodys)
{
    uint32_t bodyLen = 0;
    for (auto &v : bodys) {
        bodyLen += v.second;
    }
    auto totalLen = 2 + 2 + 2 + bodyLen + 2;
    hid_tx_buf[0] = 0;
    auto p        = &hid_tx_buf[1];
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
    uint8_t *theKey = (uint8_t *) &communication_key;
    uint32_t keyIdx = 0;
    for (unsigned i = 4; i < totalLen - 2; ++i, keyIdx++) {
        p[i] ^= theKey[keyIdx & 0x3];
    }

    auto crc                   = crc16(p, 2 + 2 + 2 + bodyLen);
    p[2 + 2 + 2 + bodyLen + 0] = (crc >> 0) & 0xFF;
    p[2 + 2 + 2 + bodyLen + 1] = (crc >> 8) & 0xFF;
    //QByteArray bin((char*)p,totalLen);
    //qDebug() << __func__ << bin.toHex() << ", size " << bin.size();

    hid_write(handle, hid_tx_buf, 65);
}
void makeDeviceInitCmd(const char *area, uint8_t mode)
{
    char buf[16 + 1];
    memset(buf, 0, sizeof(buf));
    strncpy(buf, area, 15);
    buf[16] = mode;
    makeCommand(JL_SU_CMD_DEVICE_INIT, {{buf, sizeof(buf)}});
}

int deviceUpdateZoneInit(const char *name, int mode, DeviceInitInfo &info)
{
    qDebug() << QString("send device init cmd: %1 %2").arg(name).arg(mode);

    makeDeviceInitCmd(name, mode);

    read_deice(&dev_cmd_ops);

    if (dev_cmd_ops.rsp_status == JL_SU_RSP_SUCC) {
        qDebug() << "rsp succ"; //
        qDebug() << "upgrade_addr:0x" << QString::number(dev_cmd_ops.init.upgrade_addr, 16);
        qDebug() << "upgrade_len:" << dev_cmd_ops.init.upgrade_len;
        qDebug() << "flash_alignsize:" << dev_cmd_ops.init.flash_alignsize;
        qDebug() << "upgrade_eoffset:" << dev_cmd_ops.init.upgrade_eoffset;
        info.eraseUnitSize = dev_cmd_ops.init.flash_alignsize;
        info.zoneAddr = dev_cmd_ops.init.upgrade_addr;
        info.flashEoffsetSize = dev_cmd_ops.init.upgrade_eoffset;
        info.upgradeLen = dev_cmd_ops.init.upgrade_len;
    } else {
        qDebug() << "rsp error" << dev_cmd_ops.rsp_status;
    }
    return 0;
}

bool isReplyFlashCrcCmd(const QByteArray &bin, uint8_t &rspStatus, uint32_t blockCount,
                        uint16_t *crcList)
{
    //qDebug() << __func__ << bin.toHex() << ", size " << bin.size();
    if (uint8_t(bin[0]) != JL_SU_CMD_FLASH_CRC || uint8_t(bin[1]) != JL_SU_RSP_SUCC) {
        rspStatus = uint8_t(bin[1]);
        return false;
    }
    memcpy(crcList, bin.data() + 2, blockCount * 2);
    return true;
}

bool fetchChipCrcList(uint32_t address, uint32_t blockCount, uint32_t blockSize,
                      uint16_t *crcList)
{

    while (blockCount > 0) {
        uint32_t readCount = blockCount > JL_MAX_CRC_LIST_COUNT ? JL_MAX_CRC_LIST_COUNT
                             : blockCount;
        int len = readCount * blockSize;

        makeCommand(JL_SU_CMD_FLASH_CRC, {{&address, 4}, {&len, 4}, {&blockSize, 4}});

        read_deice(&dev_cmd_ops);
        QByteArray cmd;
        cmd.resize(2 + dev_cmd_ops.cmd_len);
        memcpy(cmd.data(), hid_rx_buf + 4, 2 + dev_cmd_ops.cmd_len);
        uint8_t rspStatus;
        // parse result
        if (!isReplyFlashCrcCmd(cmd, rspStatus, readCount, crcList)) {
            qDebug() << QString("[Error, invalid reply, rspStatus %1]").arg(uint32_t(rspStatus));
            return false;
        }

        address += readCount * blockSize;
        blockCount -= readCount;
        crcList += readCount;
    }

    return true;
}

bool isReplyEraseCmd(const QByteArray &bin, uint8_t &rspStatus)
{
    //qDebug() << __func__ << bin.toHex() << ", size " << bin.size();

    if (uint8_t(bin[0]) != JL_SU_CMD_ERASE) {
        return false;
    }
    rspStatus = uint8_t(bin[1]);
    return true;
}
bool doEraseFlash(uint32_t address, uint32_t len)
{
    uint32_t type = 0;
    if (len == 64 * 1024) {
        type = JL_ERASE_TYPE_BLOCK;
    } else if (len == 4096) {
        type = JL_ERASE_TYPE_SECTOR;
    } else if (len == 256) {
        type = JL_ERASE_TYPE_PAGE;
    }
    makeCommand(JL_SU_CMD_ERASE, {{&address, 4}, {&type, 4}});
    read_deice(&dev_cmd_ops);
    QByteArray cmd;
    cmd.resize(2 + dev_cmd_ops.cmd_len);
    memcpy(cmd.data(), hid_rx_buf + 4, 2 + dev_cmd_ops.cmd_len);

    uint8_t rspStatus;
    if (!isReplyEraseCmd(cmd, rspStatus)) {
        qDebug() << "invalid erase reply";
        return false;
    }
    if (rspStatus != JL_SU_RSP_SUCC) {
        qDebug() << QString("erase reply %1").arg(rspStatus);
        return false;
    }
    return true;
}
bool eraseFlash(uint32_t address, uint32_t len)
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


void makeWriteCmd(uint32_t address, uint32_t len,
                  const void *content)
{
    return makeCommand(JL_SU_CMD_WRITE, {{&address, 4}, {&len, 4}, {content, len}});
}
bool isReplyWriteCmd(const QByteArray &bin, uint8_t &rspStatus)
{
    //qDebug() << __func__ << bin.toHex() << ", size " << bin.size();

    if (uint8_t(bin[0]) != JL_SU_CMD_WRITE) {
        return false;
    }
    rspStatus = uint8_t(bin[1]);
    return true;
}
bool doWriteFlash(const void *buffer, uint32_t address, uint32_t len)
{
    makeWriteCmd(address, len, buffer);

    read_deice(&dev_cmd_ops);
    QByteArray cmd;
    cmd.resize(2 + dev_cmd_ops.cmd_len);
    memcpy(cmd.data(), hid_rx_buf + 4, 2 + dev_cmd_ops.cmd_len);

    uint8_t rspStatus;
    if (!isReplyWriteCmd(cmd, rspStatus)) {
        qDebug() << "invalid write reply";
        return false;
    }
    if (rspStatus != JL_SU_RSP_SUCC) {
        qDebug() << QString("write reply %1").arg(rspStatus);

        return false;
    }
    return true;
}

bool writeFlash(const void *_buffer, uint32_t address, uint32_t len,
                uint32_t flash_erase_unit)
{
    const uint32_t max_write_size = 64 - sizeof(JL_SECTOR_COMMAND_ITEM);
    const uint8_t *buffer = (const uint8_t *) _buffer;
    while (len > 0) {
        uint32_t writeLen = len > max_write_size ? max_write_size : len;
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
bool ensureCrcMatch(const uint16_t *fileCrcList, uint32_t addr, uint32_t blockCount,
                    uint32_t eraseUnitSize)
{
    auto ubootChipCrcList = new uint16_t[blockCount];
    // check if we write to backup area successfully
    if (!fetchChipCrcList(addr, blockCount, eraseUnitSize, ubootChipCrcList)) {
        delete[] ubootChipCrcList;
        return false;
    }

    if (memcmp(ubootChipCrcList, fileCrcList, 2 * blockCount) != 0) {
        qDebug() << "after uboot write 64K, crc mismatch";
        delete[] ubootChipCrcList;
        return false;
    }
    delete[] ubootChipCrcList;
    return true;
}
void Reboot()
{
    return makeCommand(JL_SU_CMD_REBOOT, {});
}

int main(int argc, char *argv[])
{
    if (hid_init()) {
        return -1;
    }

    // Open the device using the VID, PID,
    // and optionally the Serial number.
    handle = hid_open(0x4c4a, 0x4155, NULL);
    if (!handle) {
        printf("unable to open device\n");
        return 1;
    }

    // Set the hid_read() function to be non-blocking.
    hid_set_nonblocking(handle, 1);

    DeviceInitInfo app;

    deviceUpdateZoneInit("app_dir_head", 0, app);

    uint32_t appZoneAddr      = app.zoneAddr;
    uint32_t flashEoffsetSize = app.flashEoffsetSize;
    uint32_t eraseUnitSize    = app.eraseUnitSize;
    QString filePath = "jl_isd.bin";
    QFile fin(filePath);
    if (!fin.open(QIODevice::ReadOnly)) {
        qDebug() << QString("open file %1 failed, error %2").arg(filePath).arg(fin.errorString());
        return -1;
    }
    auto fileContent      = fin.readAll();

    uint32_t fileSize        = fileContent.size() - appZoneAddr;
    uint32_t blockCount      = (fileSize + eraseUnitSize - 1) / eraseUnitSize;
    uint32_t alignedFileSize = blockCount * eraseUnitSize;
    if (alignedFileSize != fileSize) {
        fileContent.append(alignedFileSize - fileSize, '\xFF');
    }
    auto fileBuf = fileContent.data() + appZoneAddr;

    auto fileCrcList = getBufferCrcList(fileBuf, blockCount, eraseUnitSize);

    auto chipCrcList = new uint16_t[blockCount];


    // read crc in chip
    if (!fetchChipCrcList(0 + flashEoffsetSize, blockCount, eraseUnitSize, chipCrcList)) {
        goto __reboot;
    }

    if (memcmp(fileCrcList, chipCrcList, 2 * blockCount) == 0) {
        // same crc, no need to upgrade
        qDebug() << "same crc, no need to upgrade";
        goto __reboot;
    }

    {
        auto upgradeAddr = 0 + flashEoffsetSize;
        if (!eraseFlash(upgradeAddr, eraseUnitSize)) {
            return -1;
        }

        for (uint32_t i = 1; i < blockCount; ++i) {
            uint32_t off = i * eraseUnitSize;
            if (fileCrcList[i] != chipCrcList[i]) {
                qDebug() << "should erase " << i << ", off = " << off
                         << " size = " << eraseUnitSize;
                if (!eraseFlash(upgradeAddr + off, eraseUnitSize)) {
                    return -1;
                }
                if (!writeFlash(fileBuf + off, upgradeAddr + off, eraseUnitSize, eraseUnitSize)) {
                    return -1;
                }
            }
        }

        // and finally write first unit
        if (!writeFlash(fileBuf + 0 * eraseUnitSize, upgradeAddr + 0 * eraseUnitSize, eraseUnitSize,
                        eraseUnitSize)) {
            return -1;
        }

        if (!ensureCrcMatch(fileCrcList, 0 + flashEoffsetSize, blockCount, eraseUnitSize)) {
            qDebug() << "update failed, crc mismatch";
            return -1;
        }
    }

__reboot:
    // ask for reboot
    qDebug() << "update ok";
    qDebug() << "send reboot";
    Reboot();
    delete []  chipCrcList;
    delete []  fileCrcList;
    hid_close(handle);
    QThread::sleep(10);
    /* Free static HIDAPI objects. */
    return hid_exit();

}


