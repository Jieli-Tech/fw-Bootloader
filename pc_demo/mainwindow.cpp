#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QByteArray>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    refreshSerialPorts();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnClearLog_clicked()
{
    ui->editLog->clear();
}

void MainWindow::on_btnSelectFile_clicked()
{
    auto filePath = QFileDialog::getOpenFileName(this, tr("select update file"), "",
                                                 tr("update file (*.bin)"));
    if (filePath.isEmpty()) {
        return;
    }
    QFile fin(filePath);
    if (!fin.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("open file failed"),
                             tr("open file %1 failed, error %2")
                                 .arg(filePath)
                                 .arg(fin.errorString()));
        return;
    }
    auto buf      = std::make_shared<QByteArray>(fin.readAll());
    m_fileContent = buf;
    appendLog(tr("file opened: %1").arg(filePath));
}

void MainWindow::on_btnStart_clicked()
{
    if (m_updateThread) {
        if (m_updateThread->isRunning()) {
            QMessageBox::warning(this, tr("updating"), tr("wait until finished"));
            return;
        }
        m_updateThread->deleteLater();
        m_updateThread = nullptr;
    }

    if (m_fileContent == nullptr) {
        QMessageBox::warning(this, tr("file not selected"), tr("should select file"));
        return;
    }

    int currentIndex = ui->comboBoxSerials->currentIndex();
    if (currentIndex == -1) {
        QMessageBox::warning(this, tr("no port selected"), tr("should select serial port"));
        return;
    }
    auto portName = ui->comboBoxSerials->currentData().toString();
    auto baudRate = ui->spinBoxBaudRate->value();
    auto encKey = ui->editKey->text().toUInt();


    auto thread = new UpdateThread(portName, baudRate, m_fileContent,
                                   ui->checkBoxUpdateUBoot->isChecked(),
                                   encKey);

    connect(thread, &UpdateThread::infoError, this, &MainWindow::on_infoError);
    connect(thread, &UpdateThread::infoMessage, this, &MainWindow::on_infoMessage);

    thread->start();
    m_updateThread = thread;
    appendLog(tr("update started ..."));
}

void MainWindow::on_btnRefresh_clicked()
{
    refreshSerialPorts();
}

void MainWindow::on_infoError(const QString &msg)
{
    appendLog(msg);
}

void MainWindow::on_infoMessage(const QString &msg)
{
    appendLog(msg);
}

void MainWindow::refreshSerialPorts()
{
    ui->comboBoxSerials->clear();
    auto p = QSerialPortInfo::availablePorts();
    for (auto &v : p) {
        ui->comboBoxSerials->addItem(QString("%1(%2)").arg(v.portName()).arg(v.description()),
                                     v.portName());
    }
}

void MainWindow::appendLog(const QString &msg)
{
    ui->editLog->appendPlainText(msg);
}
