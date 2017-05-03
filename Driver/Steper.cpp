#include "hFramework.h"
#include "hCloudClient.h"
#include <stdio.h>
#include "Steper.h"

Steper::Steper(){}

Steper::Steper(int k, int t_step_divider, int t_translation_ratio){
    speed_limit_enable = false;
    acceleration_limit_enable = false;
    poz_last = 0;
    poz_curent = 0;
    speed_last = 0;
    speed_curent = 0;
    acceleration_curent = 0;
    step_divider = t_step_divider;
    translation_ratio = t_translation_ratio;
}//TODO: 
// - disable motor at sart

void Steper::enableSpeedLimit(float volume){
    speed_limit_enable = true;
    speed_limit = volume;
}

void Steper::disableSpeedLimit(){
    speed_limit_enable = false;
}

void Steper::enableAccelerationLimit(float volume){
    acceleration_limit_enable = true;
    acceleration_limit = volume;
}

void Steper::disableAccelerationLimit(){
    acceleration_limit_enable = false;
}

void Steper::enableMotor(){}//TODO:

void Steper::disableMotor(){}//TODO:

void Steper::reversPolarity(){}//TODO:

float Steper::getPozytion(angle_scale type){
    switch(type){
        case deg :
        return poz_curent;
        break;
        case rad :
        return poz_curent / 180 * pi;
        break;
        case grad :
        return poz_curent / 360 * 1000;
        break;
        case rot :
        return poz_curent / 360;
        break;
    }
    return 0;
    }

float Steper::getSpeed(angle_scale type){
    switch(type){
        case deg :
        return speed_curent;
        break;
        case rad :
        return speed_curent / 180 * pi;
        break;
        case grad :
        return speed_curent / 360 * 1000;
        break;
        case rot :
        return speed_curent / 360;
        break;
    }
    return 0;
    }

float Steper::getAcceleration(angle_scale type){
    switch(type){
        case deg :
        return acceleration_curent;
        break;
        case rad :
        return acceleration_curent / 180 * pi;
        break;
        case grad :
        return acceleration_curent / 360 * 1000;
        break;
        case rot :
        return acceleration_curent / 360;
        break;
    }
    return 0;
    }

void Steper::setStepsDivider(int t_step_divider){
    step_divider = t_step_divider;
}

void Steper::setTranslationRetio(int t_translation_ratio){
    translation_ratio = t_translation_ratio;
}

void Steper::rotRel(float angle, float step_delay){}//TODO:

void Steper::rotAbs(float angle, float step_delay){}//TODO:

void Steper::rotRelConstatnSpeed(float angle, float speed){}//TODO:

void Steper::rotAbsConstatnSpeed(float angle, float speed){}//TODO:
