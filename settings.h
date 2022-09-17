#ifndef SETTINGS_H
#define SETTINGS_H

#include "myprocess.h"

#include <QDialog>
#include <QString>
#include <QStringList>

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

private:
    Ui::Settings *ui;

    MyProcess *myProcess;

    QString currentPrinterA4;
    QString currentPrinterA6;
    QString previousPrinterA4;
    QString previousPrinterA6;
    QStringList printers;

public slots:
    void showSettings(QStringList printers);

signals:
    void closeSettings(QString currentPrinterA4, QString currentPrinterA6);

private slots:
    void on_pushButton_confirm_clicked();
    void on_pushButton_cancel_clicked();
    void on_comboBox_printing_A4_currentIndexChanged(int index);
    void on_comboBox_printing_A6_currentIndexChanged(int index);
};

#endif // SETTINGS_H
