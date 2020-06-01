// Copyright John Emil Petersen III 2019-2020
// GNU Public License

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

    // PID values need to be tuned to your mount.
    PID pidAz(1, 0.5, 0.5, 0);   // PID(P, I, D, axis)
    PID pidAlt(1, 0.5, 0.5, 1);

    countAz.start(QThread::TimeCriticalPriority);
    countAlt.start(QThread::TimeCriticalPriority);
    pidAz.start(QThread::TimeCriticalPriority);
    pidAlt.start(QThread::TimeCriticalPriority);

    return a.exec();
}
