#include <globalvars.h>
#include "mainwindow.h"
#include <cmath>

int piNumber;
bool programRunning;
bool tracking = false;
int whichAxisA = 0;
int whichAxisB = 1;

// change to your latitude and longitude in units of radians
double longitude = -10*3.141592654/180;
double latitude = 25*3.141592654/180;

// pressure at your location and altitude in mB
double pressure = 1013.25;

// temperature at your location and altitude in degrees celcius
double temperature = 20;

// humidity at your location and altitude as a decimal percentage
double humidity = 0.3;

// wavelength which you wish to observe in microns
double wavelength = 0.55;

// your elevation above sea level in meters
double elevation = 0;

// encoder gpio pins
const int encoderRotateA = 22;
const int encoderRotateB = 27;

const int encoderInclineA = 23;
const int encoderInclineB = 24;

// motor gpio pins
const int pinRotateA = 19;
const int pinRotateB = 13;
const int pinInclineA = 12;
const int pinInclineB = 18;

double targetAz;
double targetAlt;
double targetZen;

// align to polaris manually, in calibrate mode, before hitting track (slew)
const double polarisRA = 37.95291667 * 3.1415927654 / 180;
const double polarisDEC = 89.2641667 * 3.1415927654 / 180;

double angleAz;
double angleAlt;
double angleZen;

int counterAz;
int counterAlt;

// gear ratio
double gearRatio = 28.8;

// number of quadrature states your incremental encoders are capable of (PPR times 4)
int quadratureStates = 40000;

double rAscension;
double declination;

double jTime1;
double jTime2;

double AzInc = 0;
double AltInc = 0;
double incremental = 32;

void actualCallback(int direction, int axis)
{
    if(axis == 0)
    {
        counterAz += direction;
        if(counterAz > quadratureStates * gearRatio || counterAz < -quadratureStates * gearRatio)
            counterAz = 0;
        angleAz = counterAz * 2 * 3.141592654 / (gearRatio * quadratureStates);

    }
    else if (axis == 1)
    {
        counterAlt += direction;
        if(counterAlt > quadratureStates * gearRatio || counterAlt < -quadratureStates * gearRatio)
            counterAlt = 0;
        angleAlt = counterAlt * 2 * 3.141592654 / (gearRatio * quadratureStates);
    }

}

// in case you want to get rid of backlash, this can get you started

/*
    if(tracking && axis == 0 && direction != lastDirectionAz)
    {
        backlashAz--;
        if(backlashAz < 1)
        {
            backlashAz = 1000;
            lastDirectionAz = direction;
        }
    }
    else if(tracking && axis == 1 && direction != lastDirectionAlt)
    {
        backlashAlt--;
        if(backlashAlt < 1)
        {
            backlashAlt = 1000;
            lastDirectionAlt = direction;
        }
    }
    */

