#include "../include/AnalogOut.hpp"
#include <iostream>

extern "C" {
#include "../../Robotics_Cape_Installer/libraries/rc_usefulincludes.h"
#include "../../Robotics_Cape_Installer/libraries/roboticscape.h"
};

using namespace bbblue;
using namespace eeros::hal;

AnalogOut::AnalogOut(std::string id, void* libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel, 
		     double scale, double offset, double rangeMin, double rangeMax, std::string unit) : 
		     ScalableOutput<double>(id, libHandle, scale, offset, rangeMin, rangeMax, unit), channel(channel) {
	rc_enable_motors();
}

double AnalogOut::get() { return 0; }

void AnalogOut::setValue(uint32_t value) { }

void AnalogOut::set(double voltage) {
// 	std::cout << " offset " << offset << std::endl;
// 	std::cout << " scale " << scale << std::endl;
	double value = (voltage - offset) / scale;
	if( value > maxOut ) value = maxOut;
	if( value < minOut ) value = minOut;
// 	std::cout << " set to " << value << std::endl;
	rc_set_motor(channel, (float)voltage);	
}

extern "C"{
	eeros::hal::ScalableOutput<double> *createAnalogOut(std::string id, void *libHandle, std::string device, uint32_t subDeviceNumber, 
							    uint32_t channel, double scale, double offset, double rangeMin, double rangeMax, std::string unit){
		return new bbblue::AnalogOut(id, libHandle, device, subDeviceNumber, channel, scale, offset, rangeMin, rangeMax, unit);
	}
}


