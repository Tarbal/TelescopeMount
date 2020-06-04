#include "pid.h"
#include "mainwindow.h"
#include "globalvars.h"
#include <cmath>
#include <thread>
#include <chrono>
#include <sys/time.h>
#include <iostream>
#include <sofa.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <QMutex>
#include <iomanip>

PID::PID(float kp, float ki, float kd, int axis)
{
    KP = kp;
    KI = ki;
    KD = kd;
    axisID = axis;
    gettimeofday(&newTime, NULL);
    oldTime = newTime;

    datetime dtime = getTime(to_iso_string(boost::posix_time::microsec_clock::universal_time()));
    iauDtf2d("UTC", dtime.year, dtime.month, dtime.day, dtime.hour, dtime.minute, dtime.second + dtime.fraction / 1e6, &jTime1, &jTime2);
    iauAtco13(rAscension, declination, 0.0, 0.0, 0.0, 0.0, jTime1, jTime2, 0.0, longitude,
                      latitude, elevation, 0.0, 0.0, pressure, temperature, humidity, wavelength, &targetAz, &targetZen, &hourAngle, &CIOdec, &CIORA, &EqOrigins);

    angleAlt = clampAlt(angleZen);

    counterAlt = (int)((angleAlt / (2 * 3.141592654)) * gearRatio * quadratureStates);

    timeInterval = 10;
    debugTime = 0;
}

double PID::returnPID(float kp, float ki, float kd, int axis)
{
    /*How long since we last calculated*/
    gettimeofday(&newTime, NULL);

    double timeChange = (double)(newTime.tv_usec - oldTime.tv_usec) / 1e6;

    /*Compute all the working error variables*/
    double error;
    double dInput;
    if(axis == 0)
    {
        error = targetAz - angleAz;
        dInput = angleAz - lastInput;
        lastInput = angleAz;
    }
    else
    {
        error = targetAlt - angleAlt;
        dInput = angleAlt - lastInput;
        lastInput = angleAlt;
    }
    errSum += (error * timeChange);

    /*Compute PID Output*/
    double output = kp * error + ki * errSum + (kd * dInput/timeChange);

    /*Remember some variables for next time*/
    lastError = error;
    oldTime = newTime;

    return output;
}

PID::datetime PID::getTime(std::string str)
{
    datetime dt;
    dt.year = stoi(str.substr(0,4));
    dt.month = stoi(str.substr(4,2));
    dt.day = stoi(str.substr(6,2));
    dt.hour = stoi(str.substr(9,2));
    dt.minute = stoi(str.substr(11,2));
    dt.second = stod(str.substr(13,2));
    dt.fraction = stod(str.substr(16,8));
    return dt;
}

void PID::report()
{
    debugTime += timeInterval;
    if(axisID==0)
    {
        return;
        std::cout << debugTime << " Az: " << counterAz << " " << targetAz * 180 / 3.141592654 << " " << angleAz * 180 / 3.141592654 << std::endl;
    }
    else
    {
//        return;
        std::cout << debugTime << " Alt: " << counterAlt << " " << targetAlt * 180 / 3.141592654 << " " << angleAlt * 180 / 3.141592654 << std::endl;
    }
}

bool PID::turnForward(int axis)
{
    double diff;
    if(axis == 0)
    {
        if(angleAz < 0)
            angleAz += 2 * 3.141592654;
        diff = targetAz - angleAz;
    }
    else
    {
        if(angleAlt < 0)
            angleAlt += 2 * 3.141592654;
        diff = targetAlt - angleAlt;
    }
     if(diff < 0)
         diff += 2 * 3.141592654;
     if(diff > 3.141592654)
          return false;
     else
          return true;
}

double PID::clampAlt(double zen)
{
    double alt = (3.141592654/2) - zen;

    if (alt < 0)
    {
        return 0;
    }
    else if (alt > 3.141592654 / 2)
        return 3.141592654 - alt;
    else return alt;
}

void PID::motorControl(double speed, int motorLead1, int motorLead2, bool forward)
{
    double difference;

    if(tracking)
    {

        difference = speed;

        if(difference > 2)
            difference = 2;
        else if (difference < 0)
        {
            difference = -difference;
            if(difference > 2)
                difference = 2;
        }
    }

    else difference = speed / 512;

    if(!forward)
    {
        set_PWM_dutycycle(piNumber, motorLead2, 0);
        set_PWM_dutycycle(piNumber, motorLead1, (int)(difference * 100));
    }
    else
    {
        set_PWM_dutycycle(piNumber, motorLead1, 0);
        set_PWM_dutycycle(piNumber, motorLead2, (int)(difference * 100));
    }

}

bool PID::whichDirection(double increment)
{
    if(increment >= 0)
        return true;
    else return false;
}

void PID::run()
{
    QMutex mutie;
    while(programRunning)
    {
        mutie.lock();

        double output;

        if(tracking)
        {
            datetime dtime = getTime(to_iso_string(boost::posix_time::microsec_clock::universal_time()));
            iauDtf2d("UTC", dtime.year, dtime.month, dtime.day, dtime.hour, dtime.minute, dtime.second + dtime.fraction / 1e6, &jTime1, &jTime2);
            iauAtco13(rAscension, declination, 0.0, 0.0, 0.0, 0.0, jTime1, jTime2, 0.0, longitude,
                      latitude, elevation, 0.0, 0.0, pressure, temperature, humidity, wavelength, &targetAz, &targetZen, &hourAngle, &CIOdec, &CIORA, &EqOrigins);

            targetAlt = clampAlt(targetZen);

            output = returnPID(KP, KI, KD, axisID);


            if(axisID == 0)
                motorControl(std::abs(output), pinRotateA, pinRotateB, turnForward(axisID));
            else if(axisID == 1)
                motorControl(std::abs(output), pinInclineA, pinInclineB, turnForward((axisID)));

//            report();

        }
        else
        {
            if(axisID == 0)
                motorControl(std::abs(AzInc), pinRotateA, pinRotateB, whichDirection(AzInc));
            else if(axisID == 1)
                motorControl(std::abs(AltInc), pinInclineA, pinInclineB, whichDirection(AltInc));
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(timeInterval));
        mutie.unlock();
    }
    MainWindow::stopAxis(axisID);
}
