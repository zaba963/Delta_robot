#include "hFramework.h"
#include "hCloudClient.h"
#include <stdio.h>
#include "StepStick.h"
#include "Steper.h"
#include <hSensor.h>

using namespace hFramework;

Steper *x;
Steper *y;
Steper *z;

void runTask(){
	x = new Steper(hSens1, 400, 1, 12);
	y = new Steper(hSens2, 400, 1, 12);
	z = new Steper(hSens3, 400, 1, 12);
	x->enableMotor();
	y->enableMotor();
	z->enableMotor();
	while(true){
		x->update(1);
		y->update(1);
		z->update(1);
		sys.delay(1);
	}
}

void hMain()
{
	sys.taskCreate(runTask);

	float angle2 = 30;

	x->rotRel(angle2, 1);
	y->rotRel(angle2, 1);
	z->rotRel(angle2, 1);
	angle2 = -angle2;
	for (;;)
	{	
		x->rotRel(angle2, 1);
		y->rotRel(angle2, 1);
		z->rotRel(angle2, 1);
		angle2 = -angle2;
		hLED1.toggle();
		sys.delay(5000);
	}
}
