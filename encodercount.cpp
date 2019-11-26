#include "encodercount.h"
#include <pigpiod_if2.h>
#include <chrono>
#include <thread>
#include <globalvars.h>
#include <boost/multiprecision/gmp.hpp>
#include <iostream>
#include <stdint.h>

using namespace boost;
using namespace multiprecision;

encoderCount::encoderCount(unsigned int gpioNumberA, unsigned int gpioNumberB, bool Az)
{
    oldLevelA = 0;
    oldLevelB = 0;
    oldAB = -1;
    pinA = gpioNumberA;
    pinB = gpioNumberB;
    whichAxis = Az;
    set_mode(piNumber, gpioNumberA, PI_INPUT);
    set_mode(piNumber, gpioNumberB, PI_INPUT);
    set_pull_up_down(piNumber, gpioNumberA, PI_PUD_UP);
    set_pull_up_down(piNumber, gpioNumberB, PI_PUD_UP);

}
mpf_float_50 encoderCount::getPosition(bool Az)
{
    if (Az)
        return counterAz * 2 * 3.141592654 / (gearRatio * quadratureStates);
    else return counterAlt * 2 * 3.141592654 / (gearRatio * quadratureStates);
}
int encoderCount::myCallback(int direction)
{
    mutie.lock();
    if(whichAxis)
    {
        counterAz += direction;
        angleAz = getPosition(true);
        std::cout << counterAz << " " << angleAz * 180 / 3.1415927 << std::endl;
    }
    else
    {
        counterAlt += direction;
        angleAlt = getPosition(false);
    }
    mutie.unlock();
    return 0;
}

uint32_t encoderCount::_pulseEx(unsigned int gpio, unsigned int level, void *user)
{
    encoderCount *mySelf = (encoderCount *) user;

    mySelf->_pulse(gpio, level);

    return 0;
}

void encoderCount::_pulse(int gpio, int level)
{
    if (gpio == pinA) levelA = level; else levelB = level;

    if (gpio != oldAB) /* debounce */
    {
       oldAB = gpio;

       if ((gpio == pinA) && (level == 1))
       {
          if (levelB) (myCallback)(1);
       }
       else if ((gpio == pinB) && (level == 1))
       {
          if (levelA) (myCallback)(-1);
       }
    }
}
void encoderCount::run()
{
    callback(piNumber, pinA, EITHER_EDGE, (CBFunc_t)_pulseEx);
    callback(piNumber, pinB, EITHER_EDGE, (CBFunc_t)_pulseEx);
}

