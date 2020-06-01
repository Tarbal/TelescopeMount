# TelescopeMount

Here is some telescope mount control software which can be used with any mount using DC motors and incremental encoders,
all controlled by a Raspberry Pi 3b+ (untested on others!). This software does not include geometric corrections for 
mechanical imperfections. If it does not point precisely, then you have some mechanical problems. 

The GUI is very simple, and you may want to build on this code. This is the last this repository will be updated.
A future version may or may not be public, but it will have it's own repository. This code is  
GNU General Public License (GNU GPL), copyright John Petersen 2020.

You simply need to change the global variables in globalvars.cpp, such as gearRatio, quadratureStates to what is 
appropriate for your mount.

Set the variable quadrature states to the number of pulses per revolution on your encoder times 4. So, if your encoders are
10,000 ppr, you set quadratureStates = 40000.

Backlash will be unique to your mount and should be measured as accurately as possible before setting the value
(in units of encoder ticks) in globalvars.cpp.

The variables pinRotateA, pinRotateB, PinInclineA, and PinInclineB correspond to the gpio pins attached to your
motor drivers' PWM connection. The same goes for the encoderRotate and encoderIncline variables.

In order to compile on a Raspberry Pi 3b+, you will need to link the appropriate libraries. I suggest you compile
within QTCreator after you have downloaded and installed the boost, pigpio, and sofa libraries. Good luck.
