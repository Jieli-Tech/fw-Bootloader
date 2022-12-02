#ifndef PHYSERIALTHREAD_H
#define PHYSERIALTHREAD_H

#include "utils/blockingqueue.h"
#include <QtSerialPort/QSerialPort>
#include <QThread>

class PhySerialThread : public QThread
{
    Q_OBJECT
public:
    PhySerialThread(BlockingQueue<std::shared_ptr<QByteArray>> &rawRemoteCmdQueue);

    void run() override;
public slots:
    void on_openRequest(const QString &portName, int baudRate);
    void on_reOpenRequest(int baudRate);
    void on_writeBuffer(std::shared_ptr<QByteArray> buffer, bool shouldReplyFake);
    void on_readReady();

private:
    BlockingQueue<std::shared_ptr<QByteArray>> &m_rawRemoteCmdQueue;
    QSerialPort m_serial;
    QByteArray m_alreadyReadBuffer;

    QString m_portName;
    int m_baudRate;
};

#endif // PHYSERIALTHREAD_H
