#ifndef COMMCMD_H
#define COMMCMD_H

#include "utils/utils.h"
#include <memory>
#include <QString>
#include <QtSerialPort/QSerialPort>

enum CommCmdTy {
    CCT_OpenPort,   // open serial port
    CCT_ReOpenPort, // re-open serial port
    CCT_ClosePort,  // close current port
    CCT_FlushPort,  // remove any pending

    CCT_Noop, // a no-op
};

enum CommCmdResultTy {
    CCRT_OpenPortSuccess, // open port success
    CCRT_OpenPortFailed,  // failed to open port
    CCRT_ReOpenPortSuccess,
    CCRT_ReOpenPortFailed,
    CCRT_FlushPortSuccess,
    CCRT_SerialNotOpened,      // serial not opened
    CCRT_SendCheckSumErr,      // check sum error
    CCRT_CheckTimeOut,         // wait for check, but timeout
    CCRT_Closed,               // close the port successfully
    CCRT_InvalidReplyCheckSum, // invalid check sum error
    CCRT_WriteFailed,
    CCRT_WriteSuccessed,
    CCRT_CommandTimeout,
    CCRT_UnknownCommand,

    CCRT_NoopSuccess, // a no-op
};

enum CommCmdProgressTy {
    CCPT_SendProgress, // send progress
    CCPT_SendWaitOK,   // wait for 'OK'
};

class CommThread;

class CommCmd : public std::enable_shared_from_this<CommCmd>
{
public:
    CommCmd(CommCmdTy ty, int token, int timeout, int waitTime);
    virtual ~CommCmd();

    bool isOpenPortCmd() const { return m_type == CCT_OpenPort; }
    bool isReOpenPortCmd() const { return m_type == CCT_ReOpenPort; }
    bool isFlushPortCmd() const { return m_type == CCT_FlushPort; }
    bool isClosePortCmd() const { return m_type == CCT_ClosePort; }
    bool isNoopCmd() const { return m_type == CCT_Noop; }

    CommCmdTy getType() const { return m_type; }
    int getTimeout() const { return m_timeout; }
    int getToken() const { return m_token; }
    int getWaitTime() const { return m_waitTime; }

    int getStallTime() const { return m_stallTime; }
    void setStallTime(int t) { m_stallTime = t; }

    std::shared_ptr<CommCmd> getPtr() { return shared_from_this(); }

    virtual CommCmdResultTy handleCommand(QSerialPort &serial, CommThread &ctx) = 0;

    virtual QByteArray toBinary() const;
    virtual std::pair<bool, CommCmdResultTy> handleReply(const QByteArray &reply, int off);

private:
    CommCmdTy m_type;
    int m_timeout;
    int m_token;
    int m_waitTime;
    int m_stallTime{0};
};

class OpenPortCommCmd : public CommCmd
{
public:
    OpenPortCommCmd(const QString &port, int baudRate, bool isForce, int token, int timeout)
        : CommCmd(CCT_OpenPort, token, timeout, 0), m_portName(port), m_baudRate(baudRate),
          m_isForce(isForce)
    {}
    ~OpenPortCommCmd() override;

    const QString &getPortName() const { return m_portName; }
    int getBaudRate() const { return m_baudRate; }
    CommCmdResultTy handleCommand(QSerialPort &serial, CommThread &ctx) override;
    bool isForce() const { return m_isForce; }

private:
    QString m_portName;
    int m_baudRate;
    bool m_isForce{false};
};

class ReOpenPortCommCmd : public CommCmd
{
public:
    ReOpenPortCommCmd(int token, int timeout) : CommCmd(CCT_ReOpenPort, token, timeout, 0) {}

    ~ReOpenPortCommCmd() override;

    CommCmdResultTy handleCommand(QSerialPort &serial, CommThread &ctx) override;

private:
};

class FlushPortCommCmd : public CommCmd
{
public:
    FlushPortCommCmd(int token, int timeout) : CommCmd(CCT_FlushPort, token, timeout, 0) {}

    ~FlushPortCommCmd() override;

    CommCmdResultTy handleCommand(QSerialPort &serial, CommThread &ctx) override;
};

class ClosePortCommCmd : public CommCmd
{
public:
    ClosePortCommCmd(int token, int timeout) : CommCmd(CCT_ClosePort, token, timeout, 0) {}
    ~ClosePortCommCmd() override;
    CommCmdResultTy handleCommand(QSerialPort &serial, CommThread &ctx) override;
};

class NoopCommCmd : public CommCmd
{
public:
    NoopCommCmd(int tag, int token, int timeout) : CommCmd(CCT_Noop, token, timeout, 0), m_tag(tag)
    {}
    ~NoopCommCmd() override;
    CommCmdResultTy handleCommand(QSerialPort &serial, CommThread &ctx) override;

    int tag() const { return m_tag; }

private:
    int m_tag;
};

//--- CommandReply
class CommandReply
{
    int m_seq;
    int m_code;
    QByteArray m_body;

public:
    CommandReply(int seq, int code, char *buf, int len)
    {
        m_seq  = seq;
        m_code = code;
        if (len != 0 && buf != nullptr) {
            m_body = QByteArray(buf, len);
        }
    }

    int getSeq() { return m_seq; }
    int getCode() { return m_code; }
    const QByteArray &getBody() const { return m_body; }
};

Q_DECLARE_METATYPE(CommCmdTy)
Q_DECLARE_METATYPE(CommCmdResultTy)

#endif // COMMCMD_H
