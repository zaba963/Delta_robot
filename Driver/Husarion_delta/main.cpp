#include "hFramework.h"
#include "hCloudClient.h"
#include <stdio.h>
#include "StepStick.h"
#include "Steper.h"
#include <hSensor.h>
#include "MatlabCom.h"
#include "Delta.h"

using namespace hFramework;

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
	auto magon = platform.ui.label("magon");
	auto magoff = platform.ui.label("magoff");
}

bool eenable = false;

void onButtonEvent(hId id, ButtonEventType type)
{
    static int cnt = 0;
	if (id == "appp" && type == ButtonEventType::Pressed){Delta::get().rotRel(JointA, 15);}
	if (id == "bppp" && type == ButtonEventType::Pressed){Delta::get().rotRel(JointB, 15);}
	if (id == "cppp" && type == ButtonEventType::Pressed){Delta::get().rotRel(JointC, 15);}
	if (id == "ammm" && type == ButtonEventType::Pressed){Delta::get().rotRel(JointA, -15);}
	if (id == "bmmm" && type == ButtonEventType::Pressed){Delta::get().rotRel(JointB, -15);}
	if (id == "cmmm" && type == ButtonEventType::Pressed){Delta::get().rotRel(JointC, -15);}
	if (id == "app" && type == ButtonEventType::Pressed){Delta::get().rotRel(JointA, 5);}
	if (id == "bpp" && type == ButtonEventType::Pressed){Delta::get().rotRel(JointB, 5);}
	if (id == "cpp" && type == ButtonEventType::Pressed){Delta::get().rotRel(JointC, 5);}
	if (id == "amm" && type == ButtonEventType::Pressed){Delta::get().rotRel(JointA, -5);}
	if (id == "bmm" && type == ButtonEventType::Pressed){Delta::get().rotRel(JointB, -5);}
	if (id == "cmm" && type == ButtonEventType::Pressed){Delta::get().rotRel(JointC, -5);}
	if (id == "ap" && type == ButtonEventType::Pressed){Delta::get().rotRel(JointA, 1);}
	if (id == "bp" && type == ButtonEventType::Pressed){Delta::get().rotRel(JointB, 1);}
	if (id == "cp" && type == ButtonEventType::Pressed){Delta::get().rotRel(JointC, 1);}
	if (id == "am" && type == ButtonEventType::Pressed){Delta::get().rotRel(JointA, -1);}
	if (id == "bm" && type == ButtonEventType::Pressed){Delta::get().rotRel(JointB, -1);}
	if (id == "cm" && type == ButtonEventType::Pressed){Delta::get().rotRel(JointC, -1);}
	if (id == "magon" && type == ButtonEventType::Pressed){Delta::get().onMagnetic();}
	if (id == "magoff" && type == ButtonEventType::Pressed){Delta::get().offMagnetic();}

	if (id == "e" && type == ButtonEventType::Pressed){
		if(eenable){
			Delta::get().enableMotors();
			eenable = false;
		}
		else{
			Delta::get().disableMotors();
			eenable = true;
		}
	}
	if (id == "s" && type == ButtonEventType::Pressed){
		Delta::get().toggleStop();
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

	MatlabComInit();
	Delta::get().home();
	Delta::get().init();
	
	for (;;)
	{	
		//platform.ui.label("l1").setText("Pozytion: A:%f\tB:%f\tC:%f\nSpeed: A:%f\tB:%f\tC:%f\nAccel: A:%f\tB:%f\tC:%f", Delta::get().getCuretntPosytion(JointA), Delta::get().getCuretntPosytion(JointB), Delta::get().getCuretntPosytion(JointC), a->getSpeed(), b->getSpeed(), c->getSpeed(), a->getAcceleration(), b->getAcceleration(), c->getAcceleration());
		platform.ui.label("l1").setText("Pozytion: A:%f\tB:%f\tC:%f", Delta::get().getCuretntPosytion(JointA), Delta::get().getCuretntPosytion(JointB), Delta::get().getCuretntPosytion(JointC));
		hLED1.toggle();
		sys.delay(300);
	}
}
