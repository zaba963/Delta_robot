#include "hFramework.h"
#include "hCloudClient.h"
#include <stdio.h>
#include "StepStick.h"
#include "Steper.h"
#include <hSensor.h>

using namespace hFramework;

void hMain()
{
	StepStick stepx(hSens1);
	Steper x(stepx, 400, 1, 12);
	x.enableMotor();
	StepStick stepy(hSens2);
	Steper y(stepy, 400, 1, 12);
	y.enableMotor();
	StepStick stepz(hSens3);
	Steper z(stepz, 400, 1, 12);
	z.enableMotor();

	float angle2 = 30;
	int iter = 0;

	x.rotRel(angle2, 1);
	y.rotRel(angle2, 1);
	z.rotRel(angle2, 1);
	angle2 = -angle2;
	for (;;)
	{	
		if(iter == 5000){
			x.rotRel(angle2, 1);
			y.rotRel(angle2, 1);
			z.rotRel(angle2, 1);
			angle2 = -angle2;
			iter = 0;
			hLED1.toggle();
		}
		iter++;
		x.update(1);
		y.update(1);
		z.update(1);
		sys.delay(1);
	}
}
