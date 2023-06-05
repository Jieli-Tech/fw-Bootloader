#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "updatethread.h"
#include <memory>
#include <QByteArray>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnClearLog_clicked();

    void on_btnSelectFile_clicked();

    void on_btnStart_clicked();

    void on_btnRefresh_clicked();

    void on_infoError(const QString &msg);

    void on_infoMessage(const QString &msg);

private:
    void refreshSerialPorts();
    void appendLog(const QString &msg);

    Ui::MainWindow *ui;
    UpdateThread *m_updateThread{nullptr};
    std::shared_ptr<QByteArray> m_fileContent;
};
#endif // MAINWINDOW_H
