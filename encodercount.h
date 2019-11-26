#ifndef ENCODERCOUNT_H
#define ENCODERCOUNT_H
#include <QtCore>
#include <globalvars.h>
#include <boost/multiprecision/gmp.hpp>
#include <functional>
#include <pigpiod_if2.h>
#include <stdint.h>

using namespace boost;
using namespace multiprecision;

class encoderCount : public QThread
{

public:
   encoderCount(unsigned int gpioA, unsigned int gpioB, bool Az);
   bool whichAxis;

   void run();

   int myCallback(int direction);
   void re_cancel(void);
   void _pulse(int gpio, int level);

   /* Need a static callback to link with C. */
   static uint32_t _pulseEx(unsigned int gpio, unsigned int level, void *user);

private:

   QMutex mutie;
   mpf_float_50 getPosition(bool Az);

   int levelA;
   int levelB;
   int oldLevelA;
   int oldLevelB;
   int oldAB;
   uint32_t counterAz;
   uint32_t counterAlt;
   int pinA;
   int pinB;

};

#endif // ENCODERCOUNT_H
