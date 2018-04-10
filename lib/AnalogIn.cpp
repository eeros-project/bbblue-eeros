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
	/* Decide from which function should be read depending on channel number.
	   ADC channel 10&11 are battery and voltage jack with voltage deviders.
	   so we call their separate functions which compensate the deviders. 
	*/
	double readVaule{};
	switch (channel) {
	  case 10:
	    readVaule = rc_battery_voltage();
	    break;
	  case 11:
	    readVaule = rc_dc_jack_voltage();
	    break;
	  default:
	    readVaule = rc_adc_volt(channel);
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
