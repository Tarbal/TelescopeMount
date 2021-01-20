//copyright John Emil Petersen III, 2019-2021... This particular file and its associated PID algorithm is for demonstrative purposes to the public only, 
//and is not permitted to be used in commercial applications without expressed written permission of John Emil Petersen III

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
    gettimeofday(&newTime, NULL);

    double timeChange = (double)(newTime.tv_usec - oldTime.tv_usec) / 1e6;

//    if(timeChange < 0)
//        return 0;

    if(axis == 0)
    {
        double error = targetAz - angleAz;
    }
    else double error = targetAlt - angleAlt;

    if(sameSign(error, lastError))
        errSumAz += (error * timeChange);
    double dErr = (error - lastError) / timeChange;
    double output = (kp * error) + (ki * errSumAz) + (kd * dErr);

    if(output > 0)
        output -= std::abs(((output - oldOutput)*timeChange)/200);
    else if(output < 0)
        output += std::abs(((output - oldOutput)*timeChange)/200);

    if(!sameSign(output, oldOutput))
        output = 0;
    report();
    lastError = error;
    oldTime = newTime;
    oldOutput = output;
    return output;

}

bool PID::sameSign(double d1, double d2)
{
    if(d1*d2 >= 0)
        return true;
    else return false;
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
        std::cout << debugTime << " Az: " << counterAz << " " << (targetAz - angleAz) * 180 / 3.141592654 << std::endl;
    }
    else
    {
//        return;
        std::cout << debugTime << " Alt: " << counterAlt << " " << (targetAlt - angleAlt) * 180 / 3.141592654 << std::endl;
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

void PID::motorControl(double speed, int axis, int motorLead1, int motorLead2, bool forward)
{
    double difference;

    if(tracking)
    {
        difference = speed/pwmRange;

        if (difference < 0)
        {
            difference = -difference;
        }

        if(difference > 1)
            difference = 1;

        std::cout << difference << std::endl;
    }

    else difference = speed / pwmRange;

    if(!forward)
    {
        set_PWM_dutycycle(piNumber, motorLead2, 0);
        set_PWM_dutycycle(piNumber, motorLead1, (int)(difference * pwmRange));
    }
    else
    {
        set_PWM_dutycycle(piNumber, motorLead1, 0);
        set_PWM_dutycycle(piNumber, motorLead2, (int)(difference * pwmRange));
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


            bool dir = turnForward(axisID);

            output = returnPID(KP, KI, KD, axisID);


            if(axisID == 0)
                motorControl(std::abs(output), axisID, pinRotateA, pinRotateB, dir);
            else if(axisID == 1)
                motorControl(std::abs(output), axisID, pinInclineA, pinInclineB, dir);


        }
        else
        {
            if(axisID == 0)
                motorControl(std::abs(AzInc), 0, pinRotateA, pinRotateB, whichDirection(AzInc));
            else if(axisID == 1)
                motorControl(std::abs(AltInc), 1, pinInclineA, pinInclineB, whichDirection(AltInc));
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(timeInterval));
        mutie.unlock();
    }
    MainWindow::stopAxis(axisID);
}
