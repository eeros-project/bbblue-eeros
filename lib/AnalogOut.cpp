#include "../include/AnalogOut.hpp"
#include <iostream>

extern "C" {
#include <rc_usefulincludes.h>
#include <roboticscape.h>
};

using namespace bbblue;
using namespace eeros::hal;

AnalogOut::AnalogOut(std::string id, void* libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel, 
                     double scale, double offset, double rangeMin, double rangeMax, std::string unit) 
    : ScalableOutput<double>(id, libHandle, scale, offset, rangeMin, rangeMax, unit), channel(channel) {
	rc_enable_motors();
}

double AnalogOut::get() {return value * scale + offset;}

void AnalogOut::set(double voltage) {
  value = (voltage - offset) / scale;
  if (value > maxOut) value = maxOut;
  if (value < minOut) value = minOut;
  rc_set_motor(channel, (float)value);	
}

extern "C"{
  eeros::hal::ScalableOutput<double> *createAnalogOut(std::string id, void *libHandle, std::string device, uint32_t subDeviceNumber, 
                                                      uint32_t channel, double scale, double offset, double rangeMin, double rangeMax, std::string unit) {
    return new bbblue::AnalogOut(id, libHandle, device, subDeviceNumber, channel, scale, offset, rangeMin, rangeMax, unit);
  }
}


