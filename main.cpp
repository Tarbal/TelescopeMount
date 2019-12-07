#include "mainwindow.h"
#include "encodercount.h"
#include <QApplication>
#include <pigpiod_if2.h>
#include <globalvars.h>
#include "pid.h"

int main(int argc, char *argv[])
{
    programRunning = true;
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    piNumber = pigpio_start(NULL, NULL);

    encoderCount countAz(piNumber, encoderRotateA, encoderRotateB, actualCallback);
    encoderCount countAlt(piNumber, encoderInclineA, encoderInclineB, actualCallback);

    PID pidAz(0.55, 0.3, 0.9, 0);
    PID pidAlt(0.55, 0.3, 0.9, 1);

    countAz.start(QThread::TimeCriticalPriority);
    countAlt.start(QThread::TimeCriticalPriority);
    pidAz.start(QThread::TimeCriticalPriority);
    pidAlt.start(QThread::TimeCriticalPriority);

    return a.exec();
}
