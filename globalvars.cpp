#include <globalvars.h>
#include "mainwindow.h"
#include <cmath>

int piNumber;
bool programRunning;
int whichAxisA = 0;
int whichAxisB = 1;

const int encoderRotateA = 27;
const int encoderRotateB = 22;

const int encoderInclineA = 23;
const int encoderInclineB = 24;

const int pinRotateA = 13;
const int pinRotateB = 19;
const int pinInclineA = 12;
const int pinInclineB = 18;

double targetAz = 0;
double targetAlt = 0;
double targetZen = 0;

double angleAz;
double angleAlt;

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
        if(counterAlt > 40000 * 28.8 || counterAlt < -40000 * 28.8)
            counterAlt = 0;
        angleAlt = counterAlt * 2 * 3.141592654 / (gearRatio * quadratureStates);
    }

}

