#include "mainwindow.h"
#include <memory>
#include <vector>
#include <QByteArray>

#include <QApplication>

#include <QDateTime>
#include <QFile>
#include <QMutex>
#include <QTextStream>

namespace {

QMutex gLogMutex;
QFile gLogFile;
void InitGlobalLogFile()
{
    QString fileName = QString("uboot_update_log_%1.txt")
                           .arg(QDateTime::currentDateTime().toString("MMddHHmmss"));
    gLogFile.setFileName(fileName);
    gLogFile.open(QIODevice::ReadWrite | QIODevice::Append);
}

void LogMsgOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QMutexLocker locker(&gLogMutex);
    // cout << msg.toStdString() << endl;
    //Critical Resource of Code
    QByteArray localMsg = msg.toLocal8Bit();
    QString log;

    switch (type) {
    case QtDebugMsg:
        //fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        log.append(QString("Debug: %1").arg(msg));
        break;
    case QtInfoMsg:
        //fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        log.append(QString("Info: %1  %2  %3  %4")
                       .arg(localMsg.constData())
                       .arg(context.file)
                       .arg(context.line)
                       .arg(context.function));
        break;
    case QtWarningMsg:
        //fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        log.append(QString("Warning: %1  %2  %3  %4")
                       .arg(localMsg.constData())
                       .arg(context.file)
                       .arg(context.line)
                       .arg(context.function));
        break;
    case QtCriticalMsg:
        //fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        log.append(QString("Critical: %1  %2  %3  %4")
                       .arg(localMsg.constData())
                       .arg(context.file)
                       .arg(context.line)
                       .arg(context.function));
        break;
    case QtFatalMsg:
        //fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        log.append(QString("Fatal: %1  %2  %3  %4")
                       .arg(localMsg.constData())
                       .arg(context.file)
                       .arg(context.line)
                       .arg(context.function));
        abort();
    }

    QTextStream out(&gLogFile);
    out << log << "\n";
    gLogFile.flush();
}

} // namespace

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qRegisterMetaType<std::shared_ptr<uint8_t>>("std::shared_ptr<uint8_t>");
    qRegisterMetaType<std::shared_ptr<QByteArray>>("std::shared_ptr<QByteArray>");
    qRegisterMetaType<std::vector<int>>("std::vector<int>");

#ifndef MOCK_DEBUG
    InitGlobalLogFile();
    qInstallMessageHandler(LogMsgOutput);
#endif

    MainWindow w;
    w.show();
    return a.exec();
}
