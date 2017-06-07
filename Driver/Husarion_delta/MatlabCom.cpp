#include <cstddef>
#include <cstdint>
#include "hFramework.h"
#include <iostream>
#include <cstdio>
#include <vector>
#include "MatlabCom.h"
#include "Delta.h"
#include "GripperCtrl.h"

IServo &h1 = hServoModule.servo1;

void masegeTask(){
    Serial.init(115200);
	for (;;)
	{	
		MatlabCom::get().readSerial();
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

MatlabCom::MatlabCom(){
    buf = new char[buf_size];
    size = 0;
    word_in_buf = false;
    grip = new GripperCtrl(h1);
    hServoModule.enablePower();
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
    if ((int)c == 13 || (int)c == 32){}
    else if((int)c == 10){
        MatlabCom::get().cutBuf();
        word_in_buf = false;
    }
    else if((int)c == 'N' || (int)c == 'n'){
        if(word_in_buf){
            MatlabCom::get().cutBuf();
        }
        else{word_in_buf = true;}
        buf[size] = c;
        size++;
    }
    else{
        buf[size] = c;
        size++;
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
    m_instr t = {N, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false};
    char sign;
    char* t_buf;
    t_buf = new char[size-1];
    for(int i = 0; i<size;){
        for(int k = 0; k<size-1; k++)t_buf[k]=0;
        sign = buf[i];
        i++;
        int j = 0;
        while((int)buf[i] >= ',' && (int)buf[i] <= '9'){
            t_buf[j] = buf[i];
            j++;
            i++;
        }
        if(sign == 'G' || sign == 'g'){
            t.G_b = true;
            int code = (int)atoi(t_buf);
            if(code == 0){t.G = G0;}
            if(code == 4){t.G = G4;}
            if(code == 6){t.G = G6;}
            if(code == 28){t.G = G28;}
            if(code == 90){t.G = G90;}
            if(code == 91){t.G = G91;}
        }
        if(sign == 'M' || sign == 'm'){
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
        if(sign == 'A' || sign == 'a'){
            t.A = (float)atof(t_buf);
            t.A_b = true;
            }
        if(sign == 'B' || sign == 'b'){
            t.B = (float)atof(t_buf);
            t.B_b = true;
            }
        if(sign == 'C' || sign == 'c'){
            t.C = (float)atof(t_buf);
            t.C_b = true;
            }
        if(sign == 'E' || sign == 'e'){
            t.E = (float)atof(t_buf);
            t.E_b = true;
            }
        if(sign == 'F' || sign == 'f'){
            t.F = (float)atof(t_buf);
            t.F_b = true;
            }
        if(sign == 'P' || sign == 'p'){
            t.P = (float)atof(t_buf);
            t.P_b = true;
            }
        if((sign == 'N' || sign == 'n') && !t.N_b){
            t.N = (float)atof(t_buf);
            t.N_b = true;
        }
        
    }
    gcode_instr.push_back(t);
    clearBuf();
}

void MatlabCom::interBuf(){
    if(gcode_instr.size() > 0){

        switch(gcode_instr[0].G){
            case N :Serial.printf("FALT\r\n", gcode_instr[0].N);break;
            case G0 ://->full speed
                if(gcode_instr[0].A_b){Delta::get().moveFullSpeed(JointA, gcode_instr[0].A);}
                if(gcode_instr[0].B_b){Delta::get().moveFullSpeed(JointB, gcode_instr[0].B);}
                if(gcode_instr[0].C_b){Delta::get().moveFullSpeed(JointC, gcode_instr[0].C);}
                Serial.printf("OK\r\n", gcode_instr[0].N);
            break;
            case G4 ://->delay
                if(gcode_instr[0].F_b){
                    sys.delay(gcode_instr[0].F/1000);
                    Serial.printf("OK\r\n", gcode_instr[0].N);
                }
                else{
                    Serial.printf("FALT\r\n", gcode_instr[0].N);
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
                Serial.printf("OK\r\n", gcode_instr[0].N);
            break;
            case G90 ://->absolute
                Delta::get().setToAbsolute();
                Serial.printf("OK\r\n", gcode_instr[0].N);
            break;
            case G91 ://->relative
                Delta::get().setToRelative();
                Serial.printf("OK\r\n", gcode_instr[0].N);
            break;
            case M10 ://->vacume on
                grip->open();
                Serial.printf("OK\r\n", gcode_instr[0].N);
            break;
            case M11 ://->vacume off
                grip->close();
                Serial.printf("OK\r\n", gcode_instr[0].N);
            break;
            case M17 ://->enable motors
                Delta::get().enableMotors();
                Serial.printf("OK\r\n", gcode_instr[0].N);
            break;
            case M18 ://->disable motors
                Delta::get().disableMotors();
                Serial.printf("OK\r\n", gcode_instr[0].N);
            break;
            case M112 ://->estop
                Delta::get().estopDelta();
                Serial.printf("OK\r\n", gcode_instr[0].N);
            break;
            case M114 ://->return curent position
                Serial.printf("OK A%f B%f C%f\r\n", gcode_instr[0].N, Delta::get().getCuretntPosytion(JointA), Delta::get().getCuretntPosytion(JointB), Delta::get().getCuretntPosytion(JointC));
            break;
            case M124 ://->stopmotors
                Delta::get().toggleStop();
                Serial.printf("OK\r\n", gcode_instr[0].N);
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