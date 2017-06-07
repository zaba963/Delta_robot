#include <cstddef>
#include <cstdint>
#include "hFramework.h"

#ifndef GripperCrtlClass
#define GripperCrtlClass

class GripperCtrl
{
  private:
    IServo *servo;

  public:
    GripperCtrl();
    GripperCtrl(IServo &servo_t);
    int open();
    int close();
};

#endif