#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <pigpiod_if2.h>
#include <globalvars.h>
#include <boost/multiprecision/gmp.hpp>
#include <cmath>
#include <iostream>

bool MainWindow::tracking = false;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    motorSetup();
    ui->setupUi(this);
    on_calibrateButton_clicked();
}

MainWindow::~MainWindow()
{
    tracking = false;
    calibrating = false;
    programRunning = false;
    stopAxis(whichAxisA);
    stopAxis(whichAxisB);
    pigpio_stop(piNumber);
    delete ui;
}

void MainWindow::on_trackButton_clicked()
{
    stopAxis(whichAxisA);
    stopAxis(whichAxisB);

    if(!tracking)
    {
        tracking = true;
        ui->trackButton->setStyleSheet("QPushButton {color: green;}");
        calibrating = false;
        ui->calibrateButton->setStyleSheet("QPushButton {color: black;}");
        rAscension = ui->RAField->text().toDouble() * 3.141592654 / 12;
        declination = ui->decField->text().toDouble() * 3.141592654 / 180;
    }
    else
    {
        tracking = false;
        ui->trackButton->setStyleSheet("QPushButton {color: black;}");
        calibrating = true;
        ui->calibrateButton->setStyleSheet("QPushButton {color: green;}");
    }
}


void MainWindow::on_calibrateButton_clicked()
{
    stopAxis(whichAxisA);
    stopAxis(whichAxisB);

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
        rAscension = ui->RAField->text().toDouble() * 3.141592654 / 12;
        declination = ui->decField->text().toDouble() * 3.141592654 / 180;
//        convert(rAscension, declination, targetAz, targetAlt, targetZen);
    }
}

void MainWindow::motorSetup()
{
    set_mode(piNumber, pinInclineA, PI_OUTPUT);
    set_mode(piNumber, pinInclineB, PI_OUTPUT);
    set_mode(piNumber, pinRotateA, PI_OUTPUT);
    set_mode(piNumber, pinRotateB, PI_OUTPUT);

    set_PWM_range(piNumber, pinInclineA, 512);
    set_PWM_range(piNumber, pinInclineB, 512);
    set_PWM_range(piNumber, pinRotateA, 512);
    set_PWM_range(piNumber, pinRotateB, 512);
}


void MainWindow::stopAxis(int axis)
{
    if(axis == 0)
    {
        set_PWM_dutycycle(piNumber, pinRotateA, 0);
        set_PWM_dutycycle(piNumber, pinRotateB, 0);
    }
    if(axis == 1)
    {
        set_PWM_dutycycle(piNumber, pinInclineA, 0);
        set_PWM_dutycycle(piNumber, pinInclineB, 0);
    }
}

void MainWindow::on_upButton_clicked()
{
    if(!tracking)
        AltInc += incremental;
}

void MainWindow::on_downButton_clicked()
{
    if(!tracking)
        AltInc -= incremental;
}

void MainWindow::on_rightButton_clicked()
{
    if(!tracking)
        AzInc += incremental;
}

void MainWindow::on_leftButton_clicked()
{
    if(!tracking)
        AzInc -= incremental;
}
