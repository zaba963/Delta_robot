#include <cstddef>
#include <cstdint>
#include "hFramework.h"
#include "hCloudClient.h"
#include <iostream>
#include <cstdio>
#include <vector>
#include "MatlabCom.h"
#include "Delta.h"

void masegeTask(){
	for (;;)
	{	
		MatlabCom::get().readSerial();
		if(MatlabCom::get().isWorldInBuf()){
			MatlabCom::get().cutBuf();
		}
	}
}

void moveTask(){
	for (;;){
		MatlabCom::get().interBuf();
		sys.delay(100);
	}
}

void MatlabComInit(){
    sys.taskCreate(masegeTask);
	sys.taskCreate(moveTask);
}

void MatlabComTask(){
    Serial.init(115200);
    while(true){
        if(!MatlabCom::get().isWorldInBuf()){
            MatlabCom::get().readSerial();
        }
    }
}

MatlabCom::MatlabCom(){
    buf = new char[buf_size];
    size = 0;
    word_in_buf = false;
    sys.taskCreate(MatlabComTask);
}

MatlabCom & MatlabCom::get(){
    static MatlabCom s;
    return s;
}

bool MatlabCom::isWorldInBuf(){
    return word_in_buf;
}

void MatlabCom::readSerial(){
    char c;
    Serial.read(&c, 1);
    if((int)c != 10 && size<buf_size){
        buf[size] = c;
        size++;
    }
    else{
        buf[size] = c;
        if(size == 0)
            word_in_buf = false;
        else
            word_in_buf = true;
    }
}

void MatlabCom::clearBuf(){
    for(int i = 0; i<128; i++){
        buf[i] = 0;
    }
    size = 0;
    word_in_buf = false;
}

int MatlabCom::getBufSize(){
    return size;
}

void MatlabCom::cutBuf(){
    m_instr t = {N, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, operation_iter};
    operation_iter++;
    char sign;
    char* t_buf;
    t_buf = new char[size-1];
    for(int i = 0; i<size-1; i++)t_buf[i]=0;
    for(int i = 0; i<size; i++){
        sign = buf[i];
        i++;
        int j = 0;
        while(buf[i]!=(int)" " && i<size){
            t_buf[j] = buf[i];
            j++;
            i++;
        }
        i++;
        if(strcmp(&sign, "G") == 0){
            t.G_b = true;
            int code = (int)atoi(t_buf);
            if(code == 0){t.G = G0;}
            if(code == 4){t.G = G4;}
            if(code == 6){t.G = G6;}
            if(code == 28){t.G = G28;}
            if(code == 90){t.G = G90;}
            if(code == 91){t.G = G91;}
        }
        if(strcmp(&sign, "M") == 0){
            t.G_b = true;
            int code = (int)atoi(t_buf);
            if(code == 10){t.G = M10;}
            if(code == 11){t.G = M11;}
            if(code == 17){t.G = M17;}
            if(code == 18){t.G = M18;}
            if(code == 112){t.G = M112;}
            if(code == 114){t.G = M114;}
            if(code == 124){t.G = M124;}
        }
        if(strcmp(&sign, "A") == 0){t.A = (float)atof(t_buf);t.A_b = true;}
        if(strcmp(&sign, "B") == 0){t.B = (float)atof(t_buf);t.B_b = true;}
        if(strcmp(&sign, "C") == 0){t.C = (float)atof(t_buf);t.C_b = true;}
        if(strcmp(&sign, "E") == 0){t.E = (float)atof(t_buf);t.E_b = true;}
        if(strcmp(&sign, "F") == 0){t.F = (float)atof(t_buf);t.F_b = true;}
        if(strcmp(&sign, "P") == 0){t.P = (float)atof(t_buf);t.P_b = true;}
    }
    gcode_instr.push_back(t);
    clearBuf();
}

void MatlabCom::interBuf(){
    if(gcode_instr.size() > 0){

        switch(gcode_instr[0].G){
            case N :Serial.printf("FALT N%d\n", gcode_instr[0].N);break;
            case G0 ://->full speed
                if(gcode_instr[0].A_b){Delta::get().moveFullSpeed(JointA, gcode_instr[0].A);}
                if(gcode_instr[0].B_b){Delta::get().moveFullSpeed(JointA, gcode_instr[0].A);}
                if(gcode_instr[0].C_b){Delta::get().moveFullSpeed(JointA, gcode_instr[0].A);}
            break;
            case G4 ://->delay
                if(gcode_instr[0].F_b){
                    sys.delay(gcode_instr[0].F/1000);
                    Serial.printf("OK N%d\n", gcode_instr[0].N);
                }
                else{
                    Serial.printf("FALT N%d\n", gcode_instr[0].N);
                }
            break;
            case G6 ://->joint operation
                if(gcode_instr[0].A_b){}//TODO:
                if(gcode_instr[0].B_b){}//TODO:
                if(gcode_instr[0].C_b){}//TODO:
            break;
            case G28 ://->home axis
                if(!gcode_instr[0].A_b && !gcode_instr[0].B_b && !gcode_instr[0].C_b){Delta::get().home(JointAll);}
                if(gcode_instr[0].A_b){Delta::get().home(JointA);}
                if(gcode_instr[0].B_b){Delta::get().home(JointB);}
                if(gcode_instr[0].C_b){Delta::get().home(JointC);}
                Serial.printf("OK N%d\n", gcode_instr[0].N);
            break;
            case G90 ://->absolute
                Delta::get().setToAbsolute();
                Serial.printf("OK N%d\n", gcode_instr[0].N);
            break;
            case G91 ://->relative
                Delta::get().setToRelative();
                Serial.printf("OK N%d\n", gcode_instr[0].N);
            break;
            case M10 ://->vacume on
                Delta::get().onMagnetic();
                Serial.printf("OK N%d\n", gcode_instr[0].N);
            break;
            case M11 ://->vacume off
                Delta::get().offMagnetic();
                Serial.printf("OK N%d\n", gcode_instr[0].N);
            break;
            case M17 ://->enable motors
                Delta::get().enableMotors();
                Serial.printf("OK N%d\n", gcode_instr[0].N);
            break;
            case M18 ://->disable motors
                Delta::get().disableMotors();
                Serial.printf("OK N%d\n", gcode_instr[0].N);
            break;
            case M112 ://->estop
                Delta::get().estopDelta();
                Serial.printf("OK N%d\n", gcode_instr[0].N);
            break;
            case M114 ://->return curent position
                Serial.printf("OK N%d A%d B%d C%d\n", gcode_instr[0].N, Delta::get().getCuretntPosytion(JointA), Delta::get().getCuretntPosytion(JointB), Delta::get().getCuretntPosytion(JointC));
            break;
            case M124 ://->stopmotors
                Delta::get().toggleStop();
                Serial.printf("OK N%d\n", gcode_instr[0].N);
            break;
        }

        if (gcode_instr.size() == 1)
        {
            gcode_instr.clear();
        }
        else
        {
            if (gcode_instr.size() > 1)
            {
                gcode_instr.erase(gcode_instr.begin());
            }
        }
    }
}