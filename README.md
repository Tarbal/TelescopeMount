# TelescopeMount

Here is some telescope mount control software which can be used with any mount using DC gear motors and incremental encoders,
all controlled by a Raspberry Pi 3b+ (untested on other versions). This software does not include geometric corrections for 
mechanical imperfections. If it does not point accurately, then you have some mechanical problems -- most likely backlash.
RA and Dec should be entered in terms of epoch 2000 after calibrating/aligning with polaris (If you are in the
southern hemisphere, set the polaris RA and DEC in globalvars.cpp to a convenient star which you wish to use to calibrate).

The GUI is very simple, and you may want to build on this code. This is the last this repository will be updated.
A future version may or may not be public, but it will have it's own repository. This code is  
GNU General Public License (GNU GPL), copyright John Emil Petersen III, 2019-2020.

You simply need to change the global variables in globalvars.cpp, such as gearRatio and quadratureStates to what is 
appropriate for your mount. You may want to measure environmental variables such as temperature with instrumentation, 
rather than updating every time and compiling.

Set the variable quadrature states to the number of pulses per revolution on your encoder times 4. So, if your encoders are
10,000 ppr, you set quadratureStates = 40000.

The variables pinRotateA, pinRotateB, PinInclineA, and PinInclineB correspond to the gpio pins attached to your
motor drivers' PWM connection. The same goes for the encoderRotate and encoderIncline variables.

In order to compile on a Raspberry Pi 3b+, you will need to link the appropriate libraries. I suggest you compile
within QTCreator after you have downloaded and installed the boost, pigpio, and sofa libraries. Then, you need to 
run pigpiod. If you want it to run automatically at startup, add the following

#!/bin/sh -e

pigpiod;
exit 0;

to your /etc/rc.local file. If it does not exist, then create it. All of this has been tested on Ubuntu 18.04 only.
Good luck.

Once you have followed every step, You still need to tune the PID parameters in the main function. I cannot stress how
important this is.
