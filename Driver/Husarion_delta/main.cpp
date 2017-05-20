#include "hFramework.h"
#include "hCloudClient.h"
#include <stdio.h>
#include "StepStick.h"
#include "Steper.h"
#include <hSensor.h>
#include "MatlabCom.h"

using namespace hFramework;

Steper *x;
Steper *y;
Steper *z;

void runTask1(){
	x = new Steper(hSens1, 400, 1, 12);
	x->enableMotor();
	float time_temp;
	while(true){
		float t = sys.getRefTime();
		x->update(t-time_temp);
		time_temp = t;
		sys.delay(1);
	}
}

void runTask2(){
	y = new Steper(hSens2, 400, 1, 12);
	y->enableMotor();
	float time_temp;
	while(true){
		float t = sys.getRefTime();
		y->update(t-time_temp);
		time_temp = t;
		sys.delay(1);
	}
}

void runTask3(){
	z = new Steper(hSens3, 400, 1, 12);
	z->enableMotor();
	//z->enableSpeedLimit(10);
	//z->enableAccelerationLimit(10);
	float time_temp;
	while(true){
		float t = sys.getRefTime();
		z->update(t-time_temp);
		time_temp = t;
		sys.delay(1);
	}
}

void cfgHandler()
{
    platform.ui.loadHtml({Resource::WEBIDE, "/ui.html"});
    auto l1 = platform.ui.label("l1");
    auto appp = platform.ui.label("appp");
	auto bppp = platform.ui.label("bppp");
	auto cppp = platform.ui.label("cppp");
	auto app = platform.ui.label("app");
	auto bpp = platform.ui.label("bpp");
	auto cpp = platform.ui.label("cpp");
	auto ap = platform.ui.label("ap");
	auto bp = platform.ui.label("bp");
	auto cp = platform.ui.label("cp");
	auto am = platform.ui.label("am");
	auto bm = platform.ui.label("bm");
	auto cm = platform.ui.label("cm");
	auto amm = platform.ui.label("amm");
	auto bmm = platform.ui.label("bmm");
	auto cmm = platform.ui.label("cmm");
	auto ammm = platform.ui.label("ammm");
	auto bmmm = platform.ui.label("bmmm");
	auto cmmm = platform.ui.label("cmmm");
	auto s = platform.ui.label("s");
	auto e = platform.ui.label("e");
}

bool estop = false;
bool eenable = false;

void onButtonEvent(hId id, ButtonEventType type)
{
    static int cnt = 0;
	if (id == "appp" && type == ButtonEventType::Pressed){x->rotRel(15);}
	if (id == "bppp" && type == ButtonEventType::Pressed){y->rotRel(15);}
	if (id == "cppp" && type == ButtonEventType::Pressed){z->rotRel(15);}
	if (id == "ammm" && type == ButtonEventType::Pressed){x->rotRel(-15);}
	if (id == "bmmm" && type == ButtonEventType::Pressed){y->rotRel(-15);}
	if (id == "cmmm" && type == ButtonEventType::Pressed){z->rotRel(-15);}
	if (id == "app" && type == ButtonEventType::Pressed){x->rotRel(5);}
	if (id == "bpp" && type == ButtonEventType::Pressed){y->rotRel(5);}
	if (id == "cpp" && type == ButtonEventType::Pressed){z->rotRel(5);}
	if (id == "amm" && type == ButtonEventType::Pressed){x->rotRel(-5);}
	if (id == "bmm" && type == ButtonEventType::Pressed){y->rotRel(-5);}
	if (id == "cmm" && type == ButtonEventType::Pressed){z->rotRel(-5);}
	if (id == "ap" && type == ButtonEventType::Pressed){x->rotRel(1);}
	if (id == "bp" && type == ButtonEventType::Pressed){y->rotRel(1);}
	if (id == "cp" && type == ButtonEventType::Pressed){z->rotRel(1);}
	if (id == "am" && type == ButtonEventType::Pressed){x->rotRel(-1);}
	if (id == "bm" && type == ButtonEventType::Pressed){y->rotRel(-1);}
	if (id == "cm" && type == ButtonEventType::Pressed){z->rotRel(-1);}

	if (id == "e" && type == ButtonEventType::Pressed){
		if(eenable){
			x->enableMotor();
			y->enableMotor();
			z->enableMotor();
			eenable = false;
		}
		else{
			x->disableMotor();
			y->disableMotor();
			z->disableMotor();
			eenable = true;
		}
	}
	if (id == "s" && type == ButtonEventType::Pressed){
		if(estop){
			x->startMotor();
			y->startMotor();
			z->startMotor();
			estop = false;
		}
		else{
			x->stopMotor();
			y->stopMotor();
			z->stopMotor();
			estop = true;
		}
	}
}

void masegeTask(){
float iter = 0;
	for (;;)
	{	

		x->rotRel(20*sin(iter*3.14/180));
		y->rotRel(20*sin(iter*3.14/180+3.14/3*2));
		z->rotRel(20*sin(iter*3.14/180+3.14/3*4));
		iter += 10;
		sys.delay(300);

		//
	}
}

void hMain()
{
	Serial.init(115200);
	sys.setSysLogDev(&devNull);
	platform.begin(&RPi);
	platform.ui.configHandler = cfgHandler;
	platform.ui.onButtonEvent = onButtonEvent;
	platform.ui.setProjectId("00ace1b842b04814");

	sys.taskCreate(runTask1);
	sys.taskCreate(runTask2);
	sys.taskCreate(runTask3);
	sys.taskCreate(masegeTask);

	//MatlabCom::get();
	for (;;)
	{	
		platform.ui.label("l1").setText("Pozytion: A:%f\tB:%f\tC:%f\nSpeed: A:%f\tB:%f\tC:%f\nAccel: A:%f\tB:%f\tC:%f", x->getPozytion(), y->getPozytion(), z->getPozytion(), x->getSpeed(), y->getSpeed(), z->getSpeed(), x->getAcceleration(), y->getAcceleration(), z->getAcceleration());
		hLED1.toggle();
		sys.delay(300);
	}
}
