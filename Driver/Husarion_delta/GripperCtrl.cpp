#include <cstddef>
#include <cstdint>
#include "hFramework.h"
#include "GripperCtrl.h"

GripperCtrl::GripperCtrl(){}

GripperCtrl::GripperCtrl(IServo &servo_t)
{
    servo = &servo_t;
    //servo->calibrate(0, 10, 100, 2000);
}

int GripperCtrl::open(){
    servo->setWidth(1800);
    sys.delay(400);
    servo->setWidth(-1);
    return 0;
}

int GripperCtrl::close(){
    servo->setWidth(2500);
    sys.delay(400);
    servo->setWidth(-1);
    return 0;
}