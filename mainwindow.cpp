#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFile>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    settingsWindow = new Settings(this);
    settings = new QSettings("MarDom", "PrintMar");
    myProcess = new MyProcess;

    connect(settingsWindow, &Settings::closeSettings, this, &MainWindow::showMainWindow);
    connect(this, &MainWindow::openSettings, settingsWindow, &Settings::showSettings);

    if(QApplication::arguments().size() == 2)
    {
        filename = QApplication::arguments().at(1);
    }

    readQSettings();

    ui->checkBox_to_remove_A4->setChecked(toRemoveA4);
    ui->checkBox_to_remove_A6->setChecked(toRemoveA6);
}

MainWindow::~MainWindow()
{
    delete settings;
    delete myProcess;
    delete ui;
}

void MainWindow::showMainWindow(QString currentPrinterA4, QString currentPrinterA6)
{
    this->show();

    this->currentPrinterA4 = currentPrinterA4;
    this->currentPrinterA6 = currentPrinterA6;
}

void MainWindow::on_pushButton_print_A4_clicked()
{
    currentPrinterA4 = "EPSON-L3250";

    printDocument(currentPrinterA4, A4);

    if(toRemoveA4)
    {
        removeFile(filename);
    }

    this->close();
}

void MainWindow::on_pushButton_print_A6_clicked()
{
    currentPrinterA6 = "EPSON-L3250";

    printDocument(currentPrinterA6, A6);

    if(toRemoveA6)
    {
        removeFile(filename);
    }

    this->close();
}

void MainWindow::on_pushButton_open_pdf_clicked()
{
    QString program = "xdg-open";
    QStringList arguments;
    arguments << filename;

    myProcess->run(program, arguments);

    this->close();
}

void MainWindow::on_pushButton_settings_clicked()
{
    this->hide();
    printers = findPrinters();
    emit openSettings(printers);
}

void MainWindow::on_pushButton_close_clicked()
{
    this->close();
}

void MainWindow::on_checkBox_to_remove_A4_clicked(bool checked)
{
    toRemoveA4 = checked ? true : false;
}


void MainWindow::on_checkBox_to_remove_A6_clicked(bool checked)
{
    toRemoveA6 = checked ? true : false;
}

QStringList MainWindow::findPrinters()
{
    QString program = "/bin/bash";
    QStringList arguments;
    arguments <<"-c" << "lpstat -p | awk '{print $2}'";

    myProcess->run(program, arguments);
    myProcess->waitForFinished();

    QString printers = myProcess->readAll().replace("\n", "");

    return printers.split("\n");
}

void MainWindow::printDocument(QString printer, paperFormat format)
{
    QString program = "lp";

    QStringList arguments;
    arguments << "-d" << printer << "-o" << "media=" + format << filename;

    myProcess->run(program, arguments);
}

void MainWindow::removeFile(QString filename)
{
    QFile file(filename);
    file.remove();
}

void MainWindow::readQSettings()
{
    toRemoveA4 = settings->value("toRemoveA4", false).toBool();
    toRemoveA6 = settings->value("toRemoveA6", false).toBool();

    QString defaultPrinter = findPrinters().at(0);

    currentPrinterA4 = settings->value("currentPrinterA4", defaultPrinter).toString();
    currentPrinterA6 = settings->value("currentPrinterA6", defaultPrinter).toString();
}

void MainWindow::saveQSettings()
{
    settings->setValue("toRemoveA4", toRemoveA4);
    settings->setValue("toRemoveA6", toRemoveA6);
    settings->setValue("currentPrinterA4", currentPrinterA4);
    settings->setValue("currentPrinterA6", currentPrinterA6);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveQSettings();
}

