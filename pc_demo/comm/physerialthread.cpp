#include "physerialthread.h"
#include "utils/utils.h"
#include <QDateTime>
#include <QDebug>
#include <QEventLoop>

PhySerialThread::PhySerialThread(BlockingQueue<std::shared_ptr<QByteArray>> &rawRemoteCmdQueue)
    : m_rawRemoteCmdQueue(rawRemoteCmdQueue)
{
    m_serial.moveToThread(this);
    moveToThread(this);
}

void PhySerialThread::run()
{
    QEventLoop loop;
    while (!isInterruptionRequested()) {
        loop.processEvents();
    }
    m_serial.close(); // should close in this thread, or may crash
    loop.processEvents();
}

void PhySerialThread::on_openRequest(const QString &portName, int baudRate)
{
    m_portName = portName;
    m_baudRate = baudRate;

    on_reOpenRequest(baudRate);

    qDebug() << "portName= " << m_portName << ", baudRate " << m_baudRate;
}

void PhySerialThread::on_reOpenRequest(int baudRate)
{
    m_serial.close();
    m_serial.disconnect();
    m_serial.setPortName(m_portName);
    m_serial.setBaudRate(baudRate);
    if (!m_serial.open(QIODevice::ReadWrite)) {
        qDebug() << "open failed: " << m_serial.errorString();
    } else {
    }
    m_serial.readAll(); // remove pending data
    qDebug() << "time = " << QDateTime::currentMSecsSinceEpoch();
    connect(&m_serial, &QSerialPort::readyRead, this, &PhySerialThread::on_readReady);
}

void PhySerialThread::on_writeBuffer(std::shared_ptr<QByteArray> buffer, bool shouldReplyFake)
{
    int writeSize = m_serial.write(buffer->data(), buffer->size());

    qDebug() << "on write buffer: " << buffer->toHex();

    if (writeSize != buffer->size()) {
        // may be closed
        return;
    }
    if (!m_serial.waitForBytesWritten(3000)) {
        return;
    }

    if (shouldReplyFake) {
        // reply an empty buffer, for reboot command
        auto buf = std::make_shared<QByteArray>();
        m_rawRemoteCmdQueue.put(buf);
    }

    return;
}

static void removeUntilFindHead(QByteArray &buf)
{
    int i        = 0;
    uint8_t *ptr = (uint8_t *) buf.data();
    while (i < buf.size() - 2) {
        // remove until we found 0xAA55
        if (ptr[i] == 0xAA && ptr[i + 1] == 0x55) {
            break;
        }
        i++;
    }
    buf = buf.mid(i);
}

void PhySerialThread::on_readReady()
{
    auto curBuf = m_serial.readAll();
    m_alreadyReadBuffer.append(curBuf);
    qDebug() << "already read buf: " << m_alreadyReadBuffer.toHex();
    if (m_alreadyReadBuffer.size() < 4) {
        return;
    }
    //  0   1   2   3
    //  2bytes  2 bytes  n-bytes   2bytes
    // [AA 55] [len  ]  [cmd_data] [CRC16]
    //
    removeUntilFindHead(m_alreadyReadBuffer);
    qDebug() << "  after remove: " << m_alreadyReadBuffer;
    // parse all packet
    while (m_alreadyReadBuffer.size() > 2 + 2 + 2) {
        uint8_t *ptr   = (uint8_t *) m_alreadyReadBuffer.data();
        int payloadLen = ptr[2] | (uint16_t(ptr[3]) << 8);
        qDebug() << "payloadLen = " << payloadLen << " , buffer.size "
                 << m_alreadyReadBuffer.size();
        if (m_alreadyReadBuffer.size() < 2 + 2 + payloadLen + 2) {
            // not enough
            return;
        }
        // check if crc16 match
        uint16_t recvCrc16 = ptr[2 + 2 + payloadLen] | (uint16_t(ptr[2 + 2 + payloadLen + 1]) << 8);

        auto realCrc16 = crc16(&ptr[0], payloadLen + 2 + 2);
        if (recvCrc16 != realCrc16) {
            // invalid packet
            qDebug() << "recv invalid packet: ";
            qDebug() << m_alreadyReadBuffer;
            // and we should remove and find next packet
            m_alreadyReadBuffer = m_alreadyReadBuffer.mid(2);
        } else {
            // we got one valid packet, and should
            auto buf = std::make_shared<QByteArray>(m_alreadyReadBuffer.data() + 4, payloadLen);
            m_rawRemoteCmdQueue.put(buf);

            // skip to next
            m_alreadyReadBuffer = m_alreadyReadBuffer.mid(2 + 2 + payloadLen + 2);
        }
        qDebug() << " after: " << m_alreadyReadBuffer.toHex();

        removeUntilFindHead(m_alreadyReadBuffer);
    }
}
