#include <globalvars.h>
#include <boost/multiprecision/gmp.hpp>

using namespace boost;
using namespace multiprecision;

int piNumber;
char *dummy1;
char *dummy2;
bool programRunning;

const int encoderRotateA = 27;
const int encoderRotateB = 6;

const int encoderInclineA = 20;
const int encoderInclineB = 21;

double gearRatio = 28.8;
int quadratureStates = 40000;

mpf_float_50 angleAz;
mpf_float_50 angleAlt;
