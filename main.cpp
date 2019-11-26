#include "mainwindow.h"
#include "encodercount.h"
#include <QApplication>
#include <pigpiod_if2.h>
#include <globalvars.h>

int main(int argc, char *argv[])
{
    programRunning = true;
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    piNumber = pigpio_start(dummy1, dummy2);

    encoderCount countAz(encoderRotateA, encoderRotateB, true);
    encoderCount countAlt(encoderInclineA, encoderInclineB, false);

    countAz.start(QThread::TimeCriticalPriority);
    countAlt.start(QThread::TimeCriticalPriority);

    return a.exec();

}
