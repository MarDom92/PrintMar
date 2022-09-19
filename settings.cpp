#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    myProcess = new MyProcess;

    previousPrinterA4 = currentPrinterA4;
    previousPrinterA6 = currentPrinterA6;
    previouspdfViewer = pdfViewer;
}

Settings::~Settings()
{
    delete myProcess;
    delete ui;
}

void Settings::showSettings(QStringList printers, QString pdfViewer)
{
    this->printers = printers;
    this->pdfViewer = pdfViewer;

    this->show();

    ui->comboBox_printing_A4->addItems(this->printers);
    ui->comboBox_printing_A6->addItems(this->printers);
    ui->lineEdit_pdf_viewer->setText(pdfViewer);
}

void Settings::on_pushButton_confirm_clicked()
{
    pdfViewer = ui->lineEdit_pdf_viewer->text();

    emit closeSettings(currentPrinterA4, currentPrinterA6, pdfViewer);
    this->close();
}


void Settings::on_pushButton_cancel_clicked()
{    
    emit closeSettings(previousPrinterA4, previousPrinterA6, previouspdfViewer);
    this->close();
}

void Settings::on_comboBox_printing_A4_currentIndexChanged(int index)
{
    currentPrinterA4 = ui->comboBox_printing_A4->currentText();
}


void Settings::on_comboBox_printing_A6_currentIndexChanged(int index)
{
    currentPrinterA6 = ui->comboBox_printing_A6->currentText();
}
