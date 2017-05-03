#include "DeltaDriver.h"

DeltaDriver::DeltaDriver(){}

DeltaDriver::DeltaDriver(float t_gear_ratio, float t_kp_up, float t_ki_up, 
float t_kd_up, float t_kp_down, float t_ki_down, float t_kd_down, 
float t_saturate){
	destiny = 0;
	encoder_imput = 0;
	kill_button = true;
	end_stop_max = false;
	end_stop_min = false;
	output = 0;
	gear_ratio = t_gear_ratio;
	kp_up = t_kp_up;
	ki_up = t_ki_up;
	kd_up = t_kd_up;
	kp_down = t_kp_down;
	ki_down = t_ki_down;
	kd_down = t_kd_down;
	saturate = t_saturate;
	integrator = 0;
}

void DeltaDriver::buttonKillButtonSet(){kill_button = true;}

void DeltaDriver::buttonEndStopMaxSet(){end_stop_max = true;}

void DeltaDriver::buttonEndStopMinSet(){end_stop_min = true;}

void DeltaDriver::buttonKillButtonReset(){kill_button = false;}

void DeltaDriver::buttonEndStopMaxReset(){end_stop_max = false;}

void DeltaDriver::buttonEndStopMinReset(){end_stop_min = false;}

float DeltaDriver::update(){
float error = destiny - encoder_imput;
if(kill_button)error = 0;
if(end_stop_max && error > 0)error = 0;
if(end_stop_min && error < 0)error = 0;

if(error > 0){
	float error_kp = error * kp_up;
	integrator += error;
	float d_t = error - last_errot;
	output = error_kp + integrator*ki_up + d_t*kd_up;
	return output;
}
else{
	float error_kp = error * kp_down;
	integrator += error;
	float d_t = error - last_errot;
	output = error_kp + integrator*ki_down + d_t*kd_down;
	return output;
}
}

void DeltaDriver::setEncoderVolume(float volume){encoder_imput = volume;}
void DeltaDriver::setInputVolume(float volume){destiny = volume;}