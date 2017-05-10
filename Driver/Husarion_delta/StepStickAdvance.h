#include "hFramework.h"
#include "hCloudClient.h"
#include <hSensor.h>
#include <stdio.h>

#ifndef HSTEPSTICKADVANCRE
#define HSTEPSTICKADVANCRE

enum step_mode{s_1, s_2, s_4, s_8, s_16, s_32};

class StepStickAdvance{
private:
    bool dir;
    int steps_caunt;
    int step_rot;
    bool senable;
    hSensor *port;
    hGPIO *mode0;
    hGPIO *mode1;
    hGPIO *mode2;
    bool polarity_revers;

    StepStickAdvance();

    void stepRot(bool direction);
public:
    StepStickAdvance(hSensor &init_port, hGPIO &mode0_pin, hGPIO &mode1_pin, hGPIO &mode2_pin);

    void steperEnable();
    void steperDesable();
    void setDirection(bool direction);
    void toggleDirection();
    void step();
    void steps(int caunt);
    void step(bool direction);
    void reversPolarity();
    int getSensorID();
    int getCurentStep();
    void changeMode(step_mode mode);

    int getSteps();
};

#endif