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
        word_in_buf = true;
    }
}

void MatlabCom::clearBuf(){
    for(int i = 0; i<128; i++){
        buf[i] = 0;
    }
}

int MatlabCom::getBufSize(){
    return size;
}