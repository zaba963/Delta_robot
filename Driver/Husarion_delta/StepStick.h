#include "hFramework.h"
#include "hCloudClient.h"
#include <hSensor.h>
#include <stdio.h>

#ifndef HSTEPSTICK
#define HSTEPSTICK

class StepStick{
private:
    bool dir;
    int steps_caunt;
    bool senable;
    hSensor *port;
    bool polarity_revers;

    StepStick();
public:
    StepStick(hSensor &init_port);

    void steperEnable();
    void steperDesable();
    void setDirection(bool direction);
    void toggleDirection();
    void step();
    void steps(int caunt);
    void step(bool direction);
    void reversPolarity();
    int getSensorID();

    int getSteps();
};

#endif