#include <globalvars.h>
#include <boost/multiprecision/gmp.hpp>
#include <iostream>

using namespace boost;
using namespace multiprecision;

int piNumber;
bool programRunning;
int whichAxisA = 0;
int whichAxisB = 1;

const int encoderRotateA = 6;
const int encoderRotateB = 27;

const int encoderInclineA = 20;
const int encoderInclineB = 21;

const int pinRotateA = 13;
const int pinRotateB = 19;
const int pinInclineA = 12;
const int pinInclineB = 18;

mpf_float_50 angleAz;
mpf_float_50 angleAlt;

mpz_int counterAz;
mpz_int counterAlt;

double gearRatio = 28.8;
int quadratureStates = 40000;

void actualCallback(int direction, int axis)
{
//    std::cout << "print anything " << std::endl;

    if(axis == 0)
    {
        counterAz += direction;
        angleAz = (mpf_float_50)counterAz * 2 * 3.141592654 / (gearRatio * quadratureStates);
        std::cout << counterAz << " " << angleAz * 180 / 3.141592654 << std::endl;
    }
    else if (axis == 1)
    {
        counterAlt += direction;
        angleAlt = (mpf_float_50)counterAlt * 2 * 3.141592654 / (gearRatio * quadratureStates);
        std::cout << "something wrong " << std::endl;
    }
    else std::cout << "wrong again " << std::endl;
}

