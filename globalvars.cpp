#include <globalvars.h>
#include "mainwindow.h"
#include <cmath>

int piNumber;
bool programRunning;
bool tracking = false;
int whichAxisA = 0;
int whichAxisB = 1;

const int encoderRotateA = 22;
const int encoderRotateB = 27;

const int encoderInclineA = 23;
const int encoderInclineB = 24;

const int pinRotateA = 19;
const int pinRotateB = 13;
const int pinInclineA = 12;
const int pinInclineB = 18;

double targetAz;
double targetAlt;
double targetZen;

const double polarisRA = 37.95291667 * 3.1415927654 / 180;
const double polarisDEC = 89.2641667 * 3.1415927654 / 180;

double angleAz;
double angleAlt;
double angleZen;

int counterAz;
int counterAlt;

double gearRatio = 28.8;
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

