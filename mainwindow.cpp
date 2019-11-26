#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <sofa.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <math.h>
#include <pigpiod_if2.h>
#include <globalvars.h>
#include <boost/multiprecision/gmp.hpp>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    motorSetup();
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    tracking = false;
    calibrating = false;
    programRunning = false;
    pigpio_stop(piNumber);
    delete ui;
}

void MainWindow::on_trackButton_clicked()
{
    stopAll();

    if(!tracking)
    {
        tracking = true;
        ui->trackButton->setStyleSheet("QPushButton {color: green;}");
        calibrating = false;
        ui->calibrateButton->setStyleSheet("QPushButton {color: black;}");
        keepDriving(ui->RAField->text().toDouble() * 3.141592654 / 180, ui->decField->text().toDouble() * 3.141592654 / 180);
    }
    else
    {
        tracking = false;
        ui->trackButton->setStyleSheet("QPushButton {color: black;}");
        calibrating = true;
        ui->calibrateButton->setStyleSheet("QPushButton {color: green;}");
    }
}

void MainWindow::convert(double rightAscension, double declination, double &azimuth, double &altitude, double &Zen)
{
    datetime dtime = getTime(to_iso_string(boost::posix_time::microsec_clock::universal_time()));
    iauDtf2d("UTC", dtime.year, dtime.month, dtime.day, dtime.hour, dtime.minute, dtime.second + dtime.fraction, &jTime1, &jTime2);
    iauAtco13(rightAscension, declination, 0.0, 0.0, 0.0, 0.0, jTime1, jTime2, 0.0, -98.339273*3.141592654/180,
              30.334595*3.141592654/180, 356.616, 0.0, 0.0, 1013.25, 16.0, 0.5, 0.59, &azimuth, &Zen, &hourAngle, &CIOdec, &CIORA, &EqOrigins);
    altitude = (3.141592654/2) - Zen;
}

void MainWindow::on_calibrateButton_clicked()
{
    stopAll();

    if(!calibrating)
    {
        calibrating = true;
        ui->calibrateButton->setStyleSheet("QPushButton {color: green;}");
        tracking = false;
        ui->trackButton->setStyleSheet("QPushButton {color: black;}");
    }
    else
    {
        calibrating = false;
        ui->calibrateButton->setStyleSheet("QPushButton {color: black;}");
        tracking = true;
        ui->trackButton->setStyleSheet("QPushButton {color: green;}");
        keepDriving(ui->RAField->text().toDouble() * 3.141592654 / 180, ui->decField->text().toDouble() * 3.141592654 / 180);
    }
}

void MainWindow::motorSetup()
{
    set_mode(piNumber, InputInclineA, PI_OUTPUT);
    set_mode(piNumber, InputInclineB, PI_OUTPUT);
    set_mode(piNumber, InputRotateA, PI_OUTPUT);
    set_mode(piNumber, InputRotateB, PI_OUTPUT);

    set_PWM_range(piNumber, InputInclineA, 512);
    set_PWM_range(piNumber, InputInclineB, 512);
    set_PWM_range(piNumber, InputRotateA, 512);
    set_PWM_range(piNumber, InputRotateB, 512);
}

void MainWindow::MotorControl(int powerPercent, int motorLead1, int motorLead2, bool forward)
{
    if(forward)
    {
        set_PWM_dutycycle(piNumber, motorLead2, 0);
        set_PWM_dutycycle(piNumber, motorLead1, powerPercent);
    }
    else
    {
        set_PWM_dutycycle(piNumber, motorLead1, 0);
        set_PWM_dutycycle(piNumber, motorLead2, powerPercent);
    }
}

void MainWindow::keepDriving(double rA, double dec)
{
    convert(rA, dec, targetAz, targetAlt, targetZen);
    if(sqrt(pow((angleAz - targetAz), 2) + pow((angleAlt - targetAlt), 2)) > 0.1)
    {
        MotorControl(250, InputRotateA, InputRotateB, true);
    }
}

void MainWindow::stopAll()
{
    set_PWM_dutycycle(piNumber, InputInclineA, 0);
    set_PWM_dutycycle(piNumber, InputInclineB, 0);
    set_PWM_dutycycle(piNumber, InputRotateA, 0);
    set_PWM_dutycycle(piNumber, InputRotateB, 0);
}
