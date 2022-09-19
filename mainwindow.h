#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "myprocess.h"
#include "settings.h"

#include <QFileDialog>
#include <QMainWindow>
#include <QPushButton>
#include <QSettings>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showMainWindow(QString currentPrinterA4, QString currentPrinterA6, QString pdfViewer);

    void on_pushButton_print_A4_clicked();

    void on_pushButton_print_A6_clicked();

    void on_pushButton_open_pdf_clicked();

    void on_pushButton_to_remove_A4_clicked(bool checked);

    void on_pushButton_to_remove_A6_clicked(bool checked);

    void on_pushButton_to_close_app_clicked(bool checked);

    void on_pushButton_settings_clicked();

    void on_pushButton_close_clicked();

private:
    Ui::MainWindow *ui;

    Settings *settingsWindow;

    QSettings *settings;

    MyProcess *myProcess;

    QString filename;

    bool toRemoveA4;
    bool toRemoveA6;
    bool closeAppAfterOpenPdf;

    QString currentPrinterA4;
    QString currentPrinterA6;
    QStringList printers;

    QString pdfViewer;

    enum paperFormat { A4, A6 };

    QStringList findPrinters();

    QString getFilenameToPrint();

    void setStateOfQPushButton(QPushButton *button, bool state);

    void printDocument(QString printer, paperFormat format);

    void removeFile(QString filename);

    void readQSettings();

    void saveQSettings();

    void closeEvent(QCloseEvent *event);

signals:
    void openSettings(QStringList printers, QString pdfViewer);
};
#endif // MAINWINDOW_H
