#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFile>
#include <QFileDialog>
#include <QString>
#include <QPushButton>
#include <QMessageBox>

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

    setStateOfQPushButton(ui->pushButton_to_remove, isFileToRemove);
    setStateOfQPushButton(ui->pushButton_color_mode, printInMonochrome);
    setStateOfQPushButton(ui->pushButton_to_close_app, closeAppAfterOpenPdf);
}

MainWindow::~MainWindow()
{
    delete settings;
    delete myProcess;
    delete ui;
}

void MainWindow::showMainWindow(QString currentPrinterA4, QString currentPrinterA6, QString pdfViewer)
{
    this->show();

    this->currentPrinterA4 = currentPrinterA4;
    this->currentPrinterA6 = currentPrinterA6;
    this->pdfViewer = pdfViewer;
}

void MainWindow::on_pushButton_print_A4_clicked()
{
    filename = getFilenameToPrint();

    printDocument(currentPrinterA4, "A4");

    if(isFileToRemove && !filename.isEmpty()) {
        removeFile(filename);
    }

    this->close();
}

void MainWindow::on_pushButton_print_A6_clicked()
{
    filename = getFilenameToPrint();

    printDocument(currentPrinterA6, "A6");

    if(isFileToRemove && !filename.isEmpty()) {
        removeFile(filename);
    }

    this->close();
}

void MainWindow::on_pushButton_open_pdf_clicked()
{
    if(pdfViewer == "") {
        QMessageBox msgBox;
        msgBox.setText("Nieznana przeglądarka plików PDF");
        msgBox.setInformativeText("Podaj nazwę programu do przeglądania plików PDF w ustawieniach i spróbuj ponownie.");
        msgBox.exec();
    } else {
        filename = getFilenameToPrint();

        if(!filename.isEmpty()) {

            QString program = pdfViewer;
            QStringList arguments;
            arguments << filename;

            myProcess->run(program, arguments);

            if(closeAppAfterOpenPdf) {
                this->close();
            }
        }
    }
}

void MainWindow::on_pushButton_to_remove_clicked(bool checked)
{
    isFileToRemove = checked ? true : false;

    setStateOfQPushButton(ui->pushButton_to_remove, checked);
}

void MainWindow::on_pushButton_to_close_app_clicked(bool checked)
{
    closeAppAfterOpenPdf = checked ? true : false;

    setStateOfQPushButton(ui->pushButton_to_close_app, checked);
}

void MainWindow::on_pushButton_color_mode_clicked(bool checked)
{
    printInMonochrome = checked ? true : false;
    setStateOfQPushButton(ui->pushButton_color_mode, checked);
}

void MainWindow::on_pushButton_settings_clicked()
{
    this->hide();
    printers = findPrinters();
    emit openSettings(printers, pdfViewer);
}

void MainWindow::on_pushButton_close_clicked()
{
    this->close();
}

QStringList MainWindow::findPrinters()
{
    QString program = "/bin/bash";
    QStringList arguments;
    arguments <<"-c" << "lpstat -p | awk '{print $2}'";

    myProcess->run(program, arguments);
    myProcess->waitForFinished();

    QString result = myProcess->readAll();
    QStringList printers = result.split("\n");

    for(int i = 0; i < printers.size(); i++) {
        if((printers.at(i) == "") && (i > 0)) {
            printers.removeAt(i);
        }
    }

    return printers;
}

QString MainWindow::getFilenameToPrint()
{
    if(filename.isEmpty()) {
        filename = QFileDialog::getOpenFileName(this,tr("Wybierz plik PDF"), "~/", tr("Pliki PDF (*.pdf)"));
    }
    return filename;
}

void MainWindow::setStateOfQPushButton(QPushButton *button, bool state)
{
    if(state) {
        button->setText("TAK");
    } else {
        button->setText("NIE");
    }
    button->setChecked(state);
}

void MainWindow::printDocument(QString printer, QString format)
{
    QString program = "lp";
    QString isMonochrome ="";

    if (printInMonochrome) {
        isMonochrome = "ColorModel=Grayscale";
    }

    QStringList arguments;
    arguments << "-d" << printer << "-o" << "media=" + format << "-o" << isMonochrome << filename;

    myProcess->run(program, arguments);
}

void MainWindow::removeFile(QString filename)
{
    QFile file(filename);
    file.remove();
}

void MainWindow::readQSettings()
{
    isFileToRemove = settings->value("isFileToRemove", false).toBool();
    printInMonochrome = settings->value("printInMonochrome", false).toBool();
    closeAppAfterOpenPdf = settings->value("closeAppAfterOpenPdf", false).toBool();

    QString defaultPrinter = findPrinters().at(0);

    if(settings->value("currentPrinterA4") != "") {
        currentPrinterA4 = settings->value("currentPrinterA4").toString();
    } else {
        currentPrinterA4 = defaultPrinter;
    }
    if(settings->value("currentPrinterA6") != "") {
        currentPrinterA6 = settings->value("currentPrinterA6", defaultPrinter).toString();
    } else {
        currentPrinterA6 = defaultPrinter;
    }

    pdfViewer = settings->value("pdfViewer", "").toString();
}

void MainWindow::saveQSettings()
{
    settings->setValue("isFileToRemove", isFileToRemove);
    settings->setValue("printInMonochrome", printInMonochrome);
    settings->setValue("closeAppAfterOpenPdf", closeAppAfterOpenPdf);
    settings->setValue("currentPrinterA4", currentPrinterA4);
    settings->setValue("currentPrinterA6", currentPrinterA6);
    settings->setValue("pdfViewer", pdfViewer);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveQSettings();
}
