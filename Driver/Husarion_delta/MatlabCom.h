#include "hFramework.h"
#include <stdio.h>

#ifndef MATLABCOMCLASS
#define MATLABCOMCLASS

const float matlab_com_time = 100;
const int buf_size = 128;

class MatlabCom {
private:

    char* buf;
    int size;
    bool word_in_buf;

    MatlabCom();
    MatlabCom(const MatlabCom &){}
public:
    static MatlabCom & get();

    bool isWorldInBuf();
    void readSerial();
    void clearBuf();
    int getBufSize();

};

#endif