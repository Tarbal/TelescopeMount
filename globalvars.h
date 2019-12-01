#ifndef GLOBALVARS_H
#define GLOBALVARS_H
#include <boost/multiprecision/gmp.hpp>
#include "encodercount.h"

using namespace boost;
using namespace multiprecision;

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

extern mpf_float_50 angleAz;
extern mpf_float_50 angleAlt;

extern mpz_int counterAz;
extern mpz_int counterAlt;

extern double gearRatio;
extern int quadratureStates;

extern int whichAxisA;  // Az = 0
extern int whichAxisB;  // Alt = 1;

void actualCallback(int direction, int axis);

#endif // GLOBALVARS_H
