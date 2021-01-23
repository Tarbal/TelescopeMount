# TelescopeMount

1/23/21: Clarification of copyright on the updated PID algorithm is now added. The license on the file
pid.pseudo is to be considered separate from the rest of this repository, as is indicated
in comments at the top of the file. It is purely provided for demonstrative purposes.

Second Update of 1/19/21: A good starting point for PID settings, assuming you use the updated algorithm, would be
P = 20000, I = 1000, and D = 26. These are the parameters I use currently. Ultimately, however, the optimal values
for these parameters will depend on a number of factors, including motor torque, power output, mount resistance to
applied torque, and damping. I suggest reading up on the subject and mathematics of PID, before even attempting 
such an endeavor. When I said "I cannot stress how important this is," I meant it. Furthermore, a good reference
for choosing the appropriate DC motor (as well as everything else related to mount design and construction)is the
following: 

Trueblood, M. and Genet, R.M., Telescope Control (Second English Edition), Willmann-Bell, Inc. (Richmond, 1997). 

If you are taking the project seriously, I would recommend another book as well:

Smart, W.M., Textbook on Spherical Astronomy (Sixth Edition, reprinted), Cambridge University Press (Cambridge, 1979).

At some point, you'll need to account for geometric errors, which are covered in the Trueblood/Genet book. Good luck.

Update 1/19/21: I cleaned up the updated PID algorithm, even though I said 8 months ago that I would never update
this repository ever again. You have to stay one step ahead of your critics...

Update 1/18/21: It has come to my attention that the PID algorithm was never updated in this version. Uploaded as 
optional files, the updated PID algorithm is the much improved version I have been using for the past 7 months.

Here is some telescope mount control software which can be used with any mount using DC gear motors and incremental encoders,
all controlled by a Raspberry Pi 3b+ (untested on other versions). This software does not include geometric corrections for 
mechanical imperfections. If it does not point accurately, then you have some mechanical problems -- most likely backlash.
RA and Dec should be entered in terms of epoch 2000 after calibrating/aligning with polaris (If you are in the
southern hemisphere, set the polaris RA and DEC in globalvars.cpp to a convenient star which you wish to use to calibrate).

The GUI is very simple, and you may want to build on this code. This is the last this repository will be updated.
A future version may or may not be public, but it will have it's own repository. This code is  
GNU General Public License (GNU GPL), copyright John Emil Petersen III, 2019-2021.

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
