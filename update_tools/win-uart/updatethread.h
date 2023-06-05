#ifndef UPDATETHREAD_H
#define UPDATETHREAD_H

#include <QThread>

#include "comm/physerialthread.h"
#include "utils/blockingqueue.h"

using std::shared_ptr;

struct DeviceInitInfo {
     uint32_t zoneAddr, upgradeLen, flashEoffsetSize, eraseUnitSize;
};

class UpdateThread : public QThread
{
    Q_OBJECT
public:
    UpdateThread(const QString &portName, int updateBaudRate,
                 const std::shared_ptr<QByteArray> &fileContent, bool _isUpdateUboot,
                 uint32_t encKey,
                 QObject *parent = nullptr);
    ~UpdateThread() override;

    void run() override;
    void runInner();

    bool fetchChipCrcList(uint32_t address, uint32_t blockCount, uint32_t blockSize,
                          uint16_t *crcList);

    bool isUpdateUboot() const { return m_isUpdateUboot; }

    bool eraseFlash(uint32_t address, uint32_t len);
    bool doEraseFlash(uint32_t address, uint32_t len);

    bool writeFlash(const void *buffer, uint32_t address, uint32_t len, uint32_t flash_erase_unit);
    bool doWriteFlash(const void *buffer, uint32_t address, uint32_t len);
    bool ensureCrcMatch(const uint16_t *fileCrcList,
                        uint32_t addr, uint32_t blockCount, uint32_t eraseUnitSize);

    std::shared_ptr<QByteArray> waitCommand();

    bool deviceUpdateZoneInit(const char *name, uint8_t type, DeviceInitInfo &info);

signals:
    void infoWriteBuffer(std::shared_ptr<QByteArray> buffer, bool shouldReplyFake);
    void infoOpenRequest(const QString &portName, int baudRate);
    void infoReopenRequest(int baudRate);
    void infoError(const QString &msg);
    void infoMessage(const QString &msg);

private:

    BlockingQueue<shared_ptr<QByteArray>> m_rawRemoteCmdQueue;
    PhySerialThread *m_serialThread{nullptr};
    QString m_portName;
    int m_updateBaudRate{9600};
    bool m_isUpdateUboot{false};
    std::shared_ptr<QByteArray> m_fileContent;
    uint32_t m_encKey{0};
};

#endif // UPDATETHREAD_H
