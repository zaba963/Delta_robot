#include <cstddef>
#include <cstdint>
#include "hFramework.h"
#include <stdio.h>
#include <iostream>
#include <cstdio>
#include <stdio.h>
#include <string.h>
#include <vector>
#include "Delta.h"
#include "Steper.h"

using namespace hFramework;

extern Steper *a;
extern Steper *b;
extern Steper *c;

void runTask1(){
	
	a->enableMotor();
	float time_temp;
	while(true){
		float t = sys.getRefTime();
		a->update(t-time_temp);
		time_temp = t;
		sys.delay(1);
	}
}

void runTask2(){
	b->enableMotor();
	float time_temp;
	while(true){
		float t = sys.getRefTime();
		b->update(t-time_temp);
		time_temp = t;
		sys.delay(1);
	}
}

void runTask3(){
	c->enableMotor();
	//c->enableSpeedLimit(10);
	//c->enableAccelerationLimit(10);
	float time_temp;
	while(true){
		float t = sys.getRefTime();
		c->update(t-time_temp);
		time_temp = t;
		sys.delay(1);
	}
}

Delta::Delta(){
    use_absolute = true;
    stopMotors = false;
    estop = false;
    a = new Steper(hSens1, 400, 1, 12);
    b = new Steper(hSens2, 400, 1, 12);
    c = new Steper(hSens3, 400, 1, 12);
}

Delta & Delta::get(){
    static Delta s;
    return s;
}

void Delta::init(){
    sys.taskCreate(runTask1);
	sys.taskCreate(runTask2);
	sys.taskCreate(runTask3);
}

void homeJointA(){
    hSens4.pin2.setIn_pu();
    bool s;
    s = hSens4.pin2.read();
    while(!s){
    a->step(true);
    sys.delay(1);
    s = hSens4.pin2.read();
    }
    a->setCurentPozytion(jointMax);
    a->rotAbs(0);
}
void homeJointB(){
    hSens4.pin3.setIn_pu();
    bool s;
    s = hSens4.pin3.read();
    while(!s){
    b->step(true);
    sys.delay(1);
    s = hSens4.pin3.read();
    }
    b->setCurentPozytion(jointMax);
    b->rotAbs(0);
}
void homeJointC(){
    hSens4.pin4.setIn_pu();
    bool s;
    s = hSens4.pin4.read();
    while(!s){
    c->step(true);
    sys.delay(1);
    s = hSens4.pin4.read();
    }
    c->setCurentPozytion(jointMax);
    c->rotAbs(0);
}

void switch_test(){
    hSens4.pin2.setIn_pu();
    hSens4.pin3.setIn_pu();
    hSens4.pin4.setIn_pu();
    while(true){
        if(!hSens4.pin2.read() && !hSens4.pin3.read() && !hSens4.pin4.read()){
            hLED2.toggle();
        }
    }
}

void Delta::home(DeltaJoint joint){
    if(!estop){
    switch(joint){
        case JointAll: 
            homeJointA();
            homeJointB();
            homeJointC();
        break;
        case JointA: homeJointA(); break;
        case JointB: homeJointB(); break;
        case JointC: homeJointC(); break;
    }
    }

    sys.taskCreate(switch_test);
}

void Delta::setToAbsolute(){
    if(!estop)
    use_absolute = true;
}

void Delta::setToRelative(){
    if(!estop)
    use_absolute = false;
}

void Delta::onMagnetic(){
    if(!estop)
    hMot1.setPower(1000);
}

void Delta::offMagnetic(){
    if(!estop)
    hMot1.setPower(0);
}

void Delta::enableMotors(){
    if(!estop){
    a->enableMotor();
    b->enableMotor();
    c->enableMotor();
    }
}

void Delta::disableMotors(){
    if(!estop){
    a->disableMotor();
    b->disableMotor();
    c->disableMotor();
    }
}

float Delta::getCuretntPosytion(DeltaJoint joint){
    switch(joint){
        case JointAll : return 0; break;
        case JointA : return a->getPozytion(); break;
        case JointB : return b->getPozytion(); break;
        case JointC : return c->getPozytion(); break;
        default : return 0; break;
    }
}

void Delta::toggleStop(){
    if(!estop){
    if(stopMotors){
        a->startMotor();
        b->startMotor();
        c->startMotor();
    }
    if(!stopMotors){
        a->stopMotor();
        b->stopMotor();
        c->stopMotor();
    }
    stopMotors = !stopMotors;
    }
}

void Delta::estopDelta(){
    estop = true;
    a->stopMotor();
    b->stopMotor();
    c->stopMotor();
}
void Delta::moveFullSpeed(DeltaJoint joint, float volume){
    if(!estop){
    switch(joint){
        case JointAll :  break;
        case JointA : if(use_absolute){a->rotAbs(volume);}else{a->rotRel(volume);} break;
        case JointB : if(use_absolute){b->rotAbs(volume);}else{b->rotRel(volume);} break;
        case JointC : if(use_absolute){c->rotAbs(volume);}else{c->rotRel(volume);} break;
        default :  break;
    }
    }
}

void Delta::rotAbs(DeltaJoint joint, float volume){
    if(!estop){
    switch(joint){
        case JointAll : a->rotAbs(volume);b->rotAbs(volume);c->rotAbs(volume); break;
        case JointA : a->rotAbs(volume); break;
        case JointB : b->rotAbs(volume); break;
        case JointC : c->rotAbs(volume); break;
        default :  break;
    }
    }
}

void Delta::rotRel(DeltaJoint joint, float volume){
    if(!estop){
    switch(joint){
        case JointAll : a->rotRel(volume);b->rotRel(volume);c->rotRel(volume); break;
        case JointA : a->rotRel(volume); break;
        case JointB : b->rotRel(volume); break;
        case JointC : c->rotRel(volume); break;
        default :  break;
    }
    }
}
