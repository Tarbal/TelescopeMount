#ifndef PID_H
#define PID_H

#include <QThread>

class PID : public QThread
{
public:
    PID(float kp, float ki, float kd, int axis);
    void motorControl(double speed, int axis, int motorLead1, int motorLead2, bool forward);
    void run();

private:
    double returnPID(float kp, float ki, float kd, int axis);
    double returnTime();
    void report();
    void whileLoop();
    bool whichDirection(double increment);
    struct datetime
    {
            int year;
            int month;
            int day;
            int hour;
            int minute;
            double second;
            double fraction;
    };

    double clampAlt(double zen);
    bool turnForward(int axis);
    datetime getTime(std::string str);

    struct timeval newTime, oldTime;

    double lastError;
    double oldOutput;

    bool sameSign(double d1, double d2);

    float KP;
    float KI;
    float KD;
    int axisID;
//    bool limitFlag;
    double lastInput;
    double hourAngle;
    double CIOdec;
    double CIORA;
    double EqOrigins;

    int timeInterval;
    int debugTime;
};

#endif // PID_H
