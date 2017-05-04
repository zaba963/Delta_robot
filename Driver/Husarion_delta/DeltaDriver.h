#include "hFramework.h"
#include "hCloudClient.h"
#include <stdio.h>

#ifndef DELTADRIVER
#define DELTADRIVER

class DeltaDriver{
private:
	float destiny;
	float encoder_imput;
	bool kill_button;
	bool end_stop_max;
	bool end_stop_min;
	float output;
	
	float gear_ratio;
	float kp_up;
	float ki_up;
	float kd_up;
	float kp_down;
	float ki_down;
	float kd_down;
	float saturate;
	float integrator;
	
	float last_time;
	float last_errot;
	
	DeltaDriver();
public:
	DeltaDriver(float t_gear_ratio, float t_kp_up, float t_ki_up, float t_kd_up, float t_kp_down, float t_ki_down, float t_kd_down, float t_saturate);
	
	void buttonKillButtonSet();
	void buttonEndStopMaxSet();
	void buttonEndStopMinSet();
	void buttonKillButtonReset();
	void buttonEndStopMaxReset();
	void buttonEndStopMinReset();
	float update();
	
	void setEncoderVolume(float volume);
	void setInputVolume(float volume);
};

#endif