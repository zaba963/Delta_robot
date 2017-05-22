#include "hFramework.h"
#include <stdio.h>
#include "MatlabCom.h"

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
    gcode_instr.G = N;
    gcode_instr.A = 0;
    gcode_instr.B = 0;
    gcode_instr.C = 0;
    gcode_instr.F = 0;
    gcode_instr.E = 0;
    gcode_instr.P = 0;
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
        size++;
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
}

int MatlabCom::getBufSize(){
    return size;
}

void MatlabCom::cutBuf(){



    clearBuf();
}

void MatlabCom::interBuf(){
    
}