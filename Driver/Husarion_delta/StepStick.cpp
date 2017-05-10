#include "hFramework.h"
#include "hCloudClient.h"
#include <stdio.h>
#include "StepStick.h"
#include <hSensor.h>

StepStick::StepStick(){}

StepStick::StepStick(hSensor &init_port){
    motor_stop = false;
    dir = false;
    senable = true;
    polarity_revers = false;
    steps_caunt = 0;
    port = &init_port;
    port->pin2.setOut();
	port->pin3.setOut();
	port->pin4.setOut();
	port->pin2.write(dir);
	port->pin4.write(senable);
}

void StepStick::steperEnable(){
    senable = false;
    port->pin4.write(senable);
}

void StepStick::steperDesable(){
    senable = true;
    port->pin4.write(senable);
}

void StepStick::steperStop(){
    motor_stop = true;
}

void StepStick::steperStart(){
    motor_stop = false;
}

void StepStick::setDirection(bool t_direction){
    direction = t_direction;
    if(polarity_revers){
        dir = !direction;
    }
    else{
        dir = direction;
    }
    port->pin2.write(dir);
}

bool StepStick::getDirection(){
    return direction;
}

void StepStick::toggleDirection(){
    dir = !dir;
    port->pin2.write(dir);
}

void StepStick::step(){
    if(!motor_stop){
    port->pin3.toggle();
    if(dir){
        if(polarity_revers){steps_caunt--;}else{steps_caunt++;}
    }
    else{
        if(polarity_revers){steps_caunt++;}else{steps_caunt--;}
    }
    }
}

void StepStick::steps(int caunt){
    for(int i =0; i<caunt; i++){
        step();
    }
}

void StepStick::step(bool direction){
    if(dir != direction && !polarity_revers){
        dir = direction;
        port->pin2.write(dir);
    }
    if(dir == direction && polarity_revers){
        dir = !direction;
        port->pin2.write(dir);
    }
    step();
}

void StepStick::reversPolarity(){
    polarity_revers = !polarity_revers;
}

int StepStick::getSensorID(){
    return port->getID();
}

int StepStick::getSteps(){
    return steps_caunt;
}