#include "hFramework.h"
#include "hCloudClient.h"
#include <stdio.h>
#include "Steper.h"
#include "StepStick.h"

Steper::Steper(){}

Steper::Steper(hSensor &init_port, int t_steps_per_revolution, int t_step_divider, int t_translation_ratio){
    controler = new StepStick(init_port);
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
    set_angle_to_move = 0;
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

void Steper::stopMotor(){
    controler->steperStop();
}

void Steper::startMotor(){
    controler->steperStart();
}

void Steper::reversPolarity(){
    controler->reversPolarity();
}

float Steper::getPozytion(angle_scale type){
    switch(type){
        case deg :
        return poz_curent / 2;
        break;
        case rad :
        return poz_curent / 180 * pi / 2;
        break;
        case grad :
        return poz_curent / 360 * 1000 / 2;
        break;
        case rot :
        return poz_curent / 360 / 2;
        break;
    }
    return 0;
    }

float Steper::getSpeed(angle_scale type){
    switch(type){
        case deg :
        return speed_curent / 2;
        break;
        case rad :
        return speed_curent / 180 * pi / 2;
        break;
        case grad :
        return speed_curent / 360 * 1000 / 2;
        break;
        case rot :
        return speed_curent / 360 / 2;
        break;
    }
    return 0;
    }

float Steper::getAcceleration(angle_scale type){
    switch(type){
        case deg :
        return acceleration_curent / 2;
        break;
        case rad :
        return acceleration_curent / 180 * pi / 2;
        break;
        case grad :
        return acceleration_curent / 360 * 1000 / 2;
        break;
        case rot :
        return acceleration_curent / 360 / 2;
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

void Steper::rotRel(float angle){
    set_angle_to_move = angle;
}

void Steper::rotAbs(float angle){
    rotRel(angle - poz_curent);
}

void Steper::moveLinear(float distance, float speed){
    if(distance_per_step != 0){
        float st = (distance/distance_per_step);
        float ti = speed/distance;
        set_angle_to_move = deg_per_step*st;
        set_step_delay = ti/st;
    }
    else if(radius != 0){
        float st = (distance/radius);
        float ti = speed/distance;
        set_angle_to_move = deg_per_step*st;
        set_step_delay = ti/st;
    }
}

void Steper::update(float time_step_ms){
    if(set_angle_to_move != 0){poz_set = abs(set_angle_to_move);}
    poz_last = poz_last_temp;
    poz_last_temp = poz_curent;
    speed_last = speed_curent;
    speed_curent = (poz_curent - poz_last)/time_step_ms*1000;
    acceleration_curent = (speed_curent - speed_last)/time_step_ms*1000;
    if(set_angle_to_move > 0)controler->setDirection(true);
    if(set_angle_to_move < 0)controler->setDirection(false);
    if(set_angle_to_move != 0 && !acceleration_limit_enable){
        if(speed_limit_enable){
            set_step_delay = deg_per_step/speed_limit*1000;
        }
        else{
            set_step_delay = minimal_delay;
            }
        

        steps = (int)(abs(set_angle_to_move)/deg_per_step);
        set_angle_to_move = 0;
        steps *= 2;
    }

    if(acceleration_limit_enable && poz_set != 0){
        set_angle_to_move = 0;
        float t = (-speed_curent + (sqrt(speed_curent*speed_curent+2*acceleration_limit*deg_per_step)))/acceleration_limit;
        if(speed_limit_enable){
            if(speed_curent > speed_limit){
                set_step_delay = deg_per_step/speed_limit*1000;
                steps = 1;
                poz_set -= deg_per_step;
                if(poz_set < deg_per_step){poz_set = 0;}
            }
            else{
                set_step_delay = t;
                steps = 1;
                poz_set -= deg_per_step;
                if(poz_set < deg_per_step){poz_set = 0;}
            }
        }
        else{
            set_step_delay = t;
            steps = 1;
            poz_set -= deg_per_step;
            if(poz_set < deg_per_step){poz_set = 0;}
        }
    }//TODO: limit trawel

    if(set_step_delay < minimal_delay){set_step_delay = minimal_delay;}
    if(steps > 0){
        controler->step();
        if(controler->getDirection()){
            poz_curent += deg_per_step;
        }
        else{
            poz_curent -= deg_per_step;
        }
        sys.delay(set_step_delay);
        steps--;
    }
}//TODO: