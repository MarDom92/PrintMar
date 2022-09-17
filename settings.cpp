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
}

Settings::~Settings()
{
    delete myProcess;
    delete ui;
}

void Settings::showSettings(QStringList printers)
{
    this->printers = printers;

    this->show();

    ui->comboBox_printing_A4->addItems(this->printers);
    ui->comboBox_printing_A6->addItems(this->printers);
}

void Settings::on_pushButton_confirm_clicked()
{
    emit closeSettings(currentPrinterA4, currentPrinterA6);
    this->close();
}


void Settings::on_pushButton_cancel_clicked()
{
    emit closeSettings(previousPrinterA4, previousPrinterA6);
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
