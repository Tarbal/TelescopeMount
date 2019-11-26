#ifndef GLOBALVARS_H
#define GLOBALVARS_H
#include <boost/multiprecision/gmp.hpp>

using namespace boost;
using namespace multiprecision;

extern int piNumber;
extern char *dummy1;
extern char *dummy2;
extern bool programRunning;

extern const int encoderRotateA;
extern const int encoderRotateB;

extern const int encoderInclineA;
extern const int encoderInclineB;

extern mpf_float_50 angleAz;
extern mpf_float_50 angleAlt;

extern double gearRatio;
extern int quadratureStates;

#endif // GLOBALVARS_H
