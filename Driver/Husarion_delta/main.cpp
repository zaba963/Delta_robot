#include "hFramework.h"
#include "hCloudClient.h"
#include <stdio.h>
#include "StepStick.h"
#include "Steper.h"
#include <hSensor.h>

using namespace hFramework;


//Steper z(stepz, 400, 1, 12);

/*
void SteperZ(){
	while(true){
		z.update(1);
		sys.delay(1);
	}
};
*/

void hMain()
{
	
	StepStick stepz(hSens3);
	//z.enableMotor();
	//sys.taskCreate(SteperZ);
	stepz.steperEnable();

	//float angle2 = 30;
	//hSens3.pin4.setOut();
	for (;;)
	{
		hLED1.toggle();
		//z.rotRel(angle2, 1);
		//angle2 = -angle2;
		//sys.delay(5000);
		stepz.step();
		//hSens3.pin4.write(true);
		sys.delay(10);
	}
}
