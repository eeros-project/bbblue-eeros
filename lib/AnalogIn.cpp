#include "../include/AnalogIn.hpp"
#include <iostream>

extern "C" {
#include <rc_usefulincludes.h>
#include <roboticscape.h>
};

using namespace bbblue;
using namespace eeros::hal;

AnalogIn::AnalogIn(std::string id,  void* libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel, double scale,
		double offset, double rangeMin, double rangeMax, std::string unit, bool getDelta) : 
	ScalableInput<double>(id, libHandle, scale, offset, rangeMin, rangeMax, unit), channel(channel) {
}

double AnalogIn::get() {
	double readVaule{};

	if(channel < 10) {
		readVaule = rc_adc_raw(channel);

	}else if(channel < 20) {
		readVaule = rc_adc_volt(channel-10);

	}else if(channel == 25) {
		readVaule = rc_dc_jack_voltage();

	}else if(channel == 26) {
		readVaule = rc_battery_voltage();
	}

	double value = (readVaule - offset) / scale;
	if( value > maxIn ) value = maxIn;
	if( value < minIn ) value = minIn;
	return value;
}


extern "C"{
	eeros::hal::ScalableInput<double> *createAnalogIn(std::string id, void* libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel, 
						     double scale, double offset, double rangeMin, double rangeMax, std::string unit){
		return new bbblue::AnalogIn(id, libHandle, device, subDeviceNumber, channel, scale, offset, rangeMin, rangeMax, unit);
	}
}
