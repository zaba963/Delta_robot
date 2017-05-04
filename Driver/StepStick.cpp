#include "hFramework.h"
#include "hCloudClient.h"
#include <stdio.h>
#include "StepStick.h"
#include <hSensor.h>

StepStick::StepStick(){}

StepStick::StepStick(hSensor &init_port){
    dir = false;
    senable = true;
    steps = 0;
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

void StepStick::setDirection(bool direction){
    dir = direction;
    port->pin2.write(dir);
}

void StepStick::toggleDirection(){
    dir = ~dir;
    port->pin2.write(dir);
}

void StepStick::step(){
    port->pin3.toggle();
    if(dir){
        steps++;
    }
    else{
        steps--;
    }
}

void StepStick::step(bool direction){
    if(dir != direction){
        dir = direction;
        port->pin2.write(dir);
    }
    port->pin3.toggle();
    if(dir){
        steps++;
    }
    else{
        steps--;
    }
}

int StepStick::getSteps(){
    return steps;
}