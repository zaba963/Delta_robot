#include "hFramework.h"
#include "hCloudClient.h"
#include <stdio.h>

#ifndef STEPERCLASS
#define STEPERCLASS

//class work on standard rotation units
//distance/angle -> deg
//speed -> deg/s
//acceleration -> deg/s^2
float pi = 3.1415926;

enum angle_scale{deg, rad, grad, rot};

class Steper{
private:

    bool speed_limit_enable;
    float speed_limit;
    bool acceleration_limit_enable;
    float acceleration_limit;

    float poz_last;
    float poz_curent;
    float speed_last;
    float speed_curent;
    float acceleration_curent;

    int step_divider;
    int translation_ratio;

    Steper();

public:

    Steper(int k, int t_step_divider, int t_translation_ratio);//TODO:
    void enableSpeedLimit(float volume);
    void disableSpeedLimit();
    void enableAccelerationLimit(float volume);
    void disableAccelerationLimit();
    void enableMotor();
    void disableMotor();
    void reversPolarity();
    float getPozytion(angle_scale type = deg);
    float getSpeed(angle_scale type = deg);
    float getAcceleration(angle_scale type = deg);
    void setStepsDivider(int t_step_divider);
    void setTranslationRetio(int t_translation_ratio);

    void rotRel(float angle, float step_delay = 10.0);
    void rotAbs(float angle, float step_delay = 10.0);
    void rotRelConstatnSpeed(float angle, float speed = 1.0);
    void rotAbsConstatnSpeed(float angle, float speed = 1.0);

};

#endif