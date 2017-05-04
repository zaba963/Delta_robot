#include "hFramework.h"
#include "hCloudClient.h"
#include <stdio.h>
#include "Steper.h"
#include "StepStick.h"

Steper::Steper(){}

Steper::Steper(StepStick &t_controler, int t_steps_per_revolution, int t_step_divider, int t_translation_ratio){
    controler = &t_controler;
    controler->steperDesable();
    speed_limit_enable = false;
    acceleration_limit_enable = false;
    poz_last = 0;
    poz_curent = 0;
    speed_last = 0;
    speed_curent = 0;
    acceleration_curent = 0;
    step_divider = t_step_divider;
    translation_ratio = t_translation_ratio;
    steps_per_revolution = t_steps_per_revolution;
    deg_per_step = 360.0/(float)(steps_per_revolution*translation_ratio*step_divider);
    distance_per_step = 0;
    radius = 0;
    steps = 0;
}

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

void Steper::enableMotor(){
    controler->steperEnable();
}

void Steper::disableMotor(){
    controler->steperDesable();
}

void Steper::reversPolarity(){
    controler->reversPolarity();
}

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

void Steper::setStepsPerRevolutio(int t_steps_per_revolution){
    steps_per_revolution = t_steps_per_revolution;
}

void Steper::setDistancePerStep(float t_distance_per_step){
    distance_per_step = t_distance_per_step;
}

void Steper::setRadius(float t_radius){
    radius = t_radius;
}

void Steper::step(bool direction){
    controler->step(direction);
}

void Steper::rotRel(float angle, float step_delay){
    set_angle_to_move = angle;
    set_step_delay = step_delay;
}

void Steper::rotAbs(float angle, float step_delay){
    rotRel(angle - poz_curent, step_delay);
}

void Steper::rotRelConstatnSpeed(float angle, float speed){}//TODO:

void Steper::rotAbsConstatnSpeed(float angle, float speed){
    rotRelConstatnSpeed(angle - poz_curent, speed);
}

void Steper::moveLinear(float distance, float speed){}//TODO:

void Steper::update(float time_step_ms){
    if(set_angle_to_move != 0){
        if(set_angle_to_move > 0)controler->setDirection(true);
        if(set_angle_to_move < 0)controler->setDirection(false);
        if(set_step_delay < minimal_delay){set_step_delay = minimal_delay;}
        steps = (int)(set_angle_to_move/deg_per_step);
        set_angle_to_move = 0;
        steps *= 2;
    }


    if(steps > 0){
        controler->step();
        sys.delay(set_step_delay);
        steps--;
        poz_last = poz_curent;
        poz_curent += deg_per_step;
        speed_last = speed_curent;
        speed_curent = (poz_curent - poz_last)/time_step_ms*1000;
        acceleration_curent = (speed_curent - speed_last)/time_step_ms*1000;
    }
}//TODO: