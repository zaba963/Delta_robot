#include "hFramework.h"
#include <stdio.h>
#include <vector>
#include "Steper.h"

#ifndef DELTACLASS
#define DELTACLASS

#define PIN1 hSens4.pin2
#define PIN2 hSens4.pin3
#define PIN3 hSens4.pin4
#define MAG hMot1

const float jointMax = 110;
const float jointMin = -30;

enum DeltaJoint{JointAll, JointA, JointB, JointC};

class Delta{
private:
    bool use_absolute;
    bool stopMotors;
    bool estop;

    Delta();
    Delta(const Delta &){}
public:
    static Delta & get();
    void init();

    void home(DeltaJoint joint = JointAll);
    void setToAbsolute();
    void setToRelative();
    void onMagnetic();
    void offMagnetic();
    void enableMotors();
    void disableMotors();
    float getCuretntPosytion(DeltaJoint joint);
    void toggleStop();
    void estopDelta();
    void moveFullSpeed(DeltaJoint joint, float volume);
    void rotAbs(DeltaJoint joint, float volume);
    void rotRel(DeltaJoint joint, float volume);
};

#endif