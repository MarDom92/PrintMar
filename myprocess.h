#ifndef MYPROCESS_H
#define MYPROCESS_H

#include <QObject>
#include <QProcess>

class MyProcess : public QObject
{
    Q_OBJECT
public:
    explicit MyProcess(QObject *parent = nullptr);

    void run(QString program, QStringList arguments);

    bool waitForFinished();

    QByteArray readAll();

private:
    QProcess *process;

signals:

};

#endif // MYPROCESS_H
