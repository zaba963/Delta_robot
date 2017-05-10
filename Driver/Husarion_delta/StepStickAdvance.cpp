#include "hFramework.h"
#include "hCloudClient.h"
#include <stdio.h>
#include "StepStickAdvance.h"
#include <hSensor.h>

 StepStickAdvance::StepStickAdvance(){}

 StepStickAdvance::StepStickAdvance(hSensor &init_port, hGPIO &mode0_pin, hGPIO &mode1_pin, hGPIO &mode2_pin){
    dir = false;
    senable = true;
    polarity_revers = false;
    steps_caunt = 0;
    step_rot = 0;
    port = &init_port;
    mode0 = &mode0_pin;
    mode1 = &mode1_pin;
    mode2 = &mode2_pin;
    mode0->setOut();
    mode1->setOut();
    mode2->setOut();
    mode0->write(false);
    mode1->write(false);
    mode2->write(false);
    port->pin2.setOut();
	port->pin3.setOut();
	port->pin4.setOut();
	port->pin2.write(dir);
	port->pin4.write(senable);
}

void  StepStickAdvance::steperEnable(){
    senable = false;
    port->pin4.write(senable);
}

void  StepStickAdvance::steperDesable(){
    senable = true;
    port->pin4.write(senable);
}

void  StepStickAdvance::setDirection(bool direction){
    if(polarity_revers){
        dir = !direction;
    }
    else{
        dir = direction;
    }
    port->pin2.write(dir);
}

void  StepStickAdvance::toggleDirection(){
    dir = !dir;
    port->pin2.write(dir);
}

void  StepStickAdvance::step(){
    port->pin3.toggle();
    if(dir){
        if(polarity_revers){steps_caunt--;stepRot(false);}else{steps_caunt++;stepRot(true);}
    }
    else{
        if(polarity_revers){steps_caunt++;stepRot(true);}else{steps_caunt--;stepRot(false);}
    }
}

void  StepStickAdvance::steps(int caunt){
    for(int i =0; i<caunt; i++){
        step();
    }
}

void StepStickAdvance::stepRot(bool direction){
    if(direction){
        step_rot++;
    }
    else{
        step_rot--;
    }
    if(step_rot<0){
        step_rot = 32;
    }
    if(step_rot>32){
        step_rot = 0;
    }
}

void StepStickAdvance::step(bool direction){
    if(dir != direction && !polarity_revers){
        dir = direction;
        port->pin2.write(dir);
    }
    if(dir == direction && polarity_revers){
        dir = !direction;
        port->pin2.write(dir);
    }
    port->pin3.toggle();
    if(dir){
        if(polarity_revers){steps_caunt--;stepRot(false);}else{steps_caunt++;stepRot(true);}
    }
    else{
        if(polarity_revers){steps_caunt++;stepRot(true);}else{steps_caunt--;stepRot(false);}
    }
}

void  StepStickAdvance::reversPolarity(){
    polarity_revers = !polarity_revers;
}

int  StepStickAdvance::getSensorID(){
    return port->getID();
}

int StepStickAdvance::getCurentStep(){
    return step_rot;
}

void StepStickAdvance::changeMode(step_mode mode){
    switch(mode){
        case s_1: mode0->write(false); mode1->write(false); mode2->write(false); break;
        case s_2: mode0->write(true); mode1->write(false); mode2->write(false); break;
        case s_4: mode0->write(false); mode1->write(true); mode2->write(false); break;
        case s_8: mode0->write(true); mode1->write(true); mode2->write(false); break;
        case s_16: mode0->write(false); mode1->write(false); mode2->write(true); break;
        case s_32: mode0->write(true); mode1->write(false); mode2->write(true); break;
    }
}

int  StepStickAdvance::getSteps(){
    return steps_caunt;
}