#include "myprocess.h"

MyProcess::MyProcess(QObject *parent)
    : QObject{parent}
{
    process = new QProcess();
}

void MyProcess::run(QString program, QStringList arguments)
{
    if(!program.isEmpty() && !arguments.isEmpty()) {
        process->start(program, arguments);
    } else {
        EXIT_FAILURE;
    }
}

bool MyProcess::waitForFinished()
{
    return process->waitForFinished();
}

QByteArray MyProcess::readAll()
{
    return process->readAll();
}
