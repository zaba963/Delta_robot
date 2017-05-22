#include "hFramework.h"
#include <stdio.h>
#include <vector>

#ifndef MATLABCOMCLASS
#define MATLABCOMCLASS

const float matlab_com_time = 100;
const int buf_size = 128;

enum gcode_com{
    N, //-> neutral
    
    G0, //->full speed
    G4, //->delay
    G6, //->joint operation
    G28, //->home axis
    G90, //->absolute
    G91, //->relative

    M10, //->vacume on
    M11, //->vacume off
    M17, //->enable motors
    M18, //->disable motors
    M112, //->estop
    M114, //->return curent position
    M124 //->stopmotors
};

struct m_instr{
    gcode_com G;
    float A;
    float B;
    float C;
    float F;
    float E;
    float P;
    unsigned int N;
};

class MatlabCom {
private:

    char* buf;
    int size;
    bool word_in_buf;
    unsigned int operation_iter;
    std::vector<m_instr> gcode_instr;

    MatlabCom();
    MatlabCom(const MatlabCom &){}
public:
    static MatlabCom & get();

    bool isWorldInBuf();
    void readSerial();
    void clearBuf();
    int getBufSize();
    void cutBuf();
    void interBuf();

};

#endif