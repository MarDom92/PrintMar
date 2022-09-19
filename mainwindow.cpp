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

    setStateOfQPushButton(ui->pushButton_to_remove_A4, toRemoveA4);
    setStateOfQPushButton(ui->pushButton_to_remove_A6, toRemoveA6);
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
    this->pdfViewer =pdfViewer;
}

void MainWindow::on_pushButton_print_A4_clicked()
{
    filename = getFilenameToPrint();

    printDocument(currentPrinterA4, "A4");

    if(toRemoveA4 && !filename.isEmpty()) {
        removeFile(filename);
    }

    this->close();
}

void MainWindow::on_pushButton_print_A6_clicked()
{
    filename = getFilenameToPrint();

    printDocument(currentPrinterA6, "A6");

    if(toRemoveA6 && !filename.isEmpty()) {
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

        QString program = pdfViewer;//"xdg-open";
        QStringList arguments;
        arguments << filename;

        myProcess->run(program, arguments);

        if(closeAppAfterOpenPdf && !filename.isEmpty()) {
            this->close();
        }
    }
}

void MainWindow::on_pushButton_to_remove_A4_clicked(bool checked)
{
    toRemoveA4 = checked ? true : false;

    if (checked) {
        setStateOfQPushButton(ui->pushButton_to_remove_A4, true);
    } else {
        setStateOfQPushButton(ui->pushButton_to_remove_A4, false);
    }
}


void MainWindow::on_pushButton_to_remove_A6_clicked(bool checked)
{
    toRemoveA6 = checked ? true : false;

    if (checked) {
        setStateOfQPushButton(ui->pushButton_to_remove_A6, true);
    } else {
        setStateOfQPushButton(ui->pushButton_to_remove_A6, false);
    }
}

void MainWindow::on_pushButton_to_close_app_clicked(bool checked)
{
    closeAppAfterOpenPdf = checked ? true : false;

    if (checked) {
        setStateOfQPushButton(ui->pushButton_to_close_app, true);
    } else {
        setStateOfQPushButton(ui->pushButton_to_close_app, false);
    }
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
    closeAppAfterOpenPdf = settings->value("closeAppAfterOpenPdf", false).toBool();

    QString defaultPrinter = findPrinters().at(0);

    currentPrinterA4 = settings->value("currentPrinterA4", defaultPrinter).toString();
    currentPrinterA6 = settings->value("currentPrinterA6", defaultPrinter).toString();

    pdfViewer = settings->value("pdfViewer", "").toString();
}

void MainWindow::saveQSettings()
{
    settings->setValue("toRemoveA4", toRemoveA4);
    settings->setValue("toRemoveA6", toRemoveA6);
    settings->setValue("closeAppAfterOpenPdf", closeAppAfterOpenPdf);
    settings->setValue("currentPrinterA4", currentPrinterA4);
    settings->setValue("currentPrinterA6", currentPrinterA6);
    settings->setValue("pdfViewer", pdfViewer);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveQSettings();
}
