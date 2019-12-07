#ifndef GLOBALVARS_H
#define GLOBALVARS_H
#include "encodercount.h"

extern int piNumber;
extern bool programRunning;

extern const int pinRotateA;
extern const int pinRotateB;
extern const int pinInclineA;
extern const int pinInclineB;

extern const int encoderRotateA;
extern const int encoderRotateB;

extern const int encoderInclineA;
extern const int encoderInclineB;

extern double targetAz;
extern double targetAlt;
extern double targetZen;

extern double angleAz;
extern double angleAlt;

extern double AltInc;
extern double AzInc;
extern double incremental;

extern int counterAz;
extern int counterAlt;

extern double gearRatio;
extern int quadratureStates;

extern int whichAxisA;  // Az = 0
extern int whichAxisB;  // Alt = 1;

extern double rAscension;
extern double declination;

extern double jTime1;
extern double jTime2;

void actualCallback(int direction, int axis);


#endif // GLOBALVARS_H
