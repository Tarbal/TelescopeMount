#ifndef ENCODERCOUNT_H
#define ENCODERCOUNT_H
#include <QThread>
#include <globalvars.h>
#include <functional>
#include <pigpiod_if2.h>
#include <stdint.h>

typedef void (*re_decoderCB_t)(int, int);

class encoderCount : public QThread
{

public:
   encoderCount(unsigned int gpioNumberA, unsigned int gpioNumberB, re_decoderCB_t callback);

   void run();

   void speedOutput();

   int levelARotate;
   int levelBRotate;

   int levelAIncline;
   int levelBIncline;

   int cba;
   int cbb;
   re_decoderCB_t myCallback;
   void CBcancel(void);
   void _pulse(int gpio, int level);

   int lastABRotate;
   int lastABIncline;

   int currentABRotate;
   int currentABIncline;

   /* Need a static callback to link with C. */
   static void _pulseEx(int pinum, unsigned int gpio, unsigned int level, uint32_t tick, void *user);


private:

   int debugAz;
   int debugAlt;
   int pinA;
   int pinB;
   int levelA;
   int levelB;
   int oldLevelA;
   int oldLevelB;
   int oldAB;
   int outcome[16] =
   {
       0, 1, -1, 0,
       -1, 0, 0, 1,
       1, 0, 0, -1,
       0, -1, 1, 0
   };

};

#endif // ENCODERCOUNT_H
