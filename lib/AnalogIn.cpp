#include "../include/AnalogIn.hpp"

extern "C" {
#include <robotcontrol.h>
};

using namespace bbblue;
using namespace eeros::hal;

AnalogIn::AnalogIn(std::string id,  void* libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel, double scale,
                   double offset, double rangeMin, double rangeMax, std::string unit, bool getDelta) 
    : ScalableInput<double>(id, libHandle, scale, offset, rangeMin, rangeMax, unit), channel(channel) { 
  rc_adc_init();
}

AnalogIn::~AnalogIn() {
  rc_adc_cleanup();
}


double AnalogIn::get() {
  double val = 0;

  if (channel < 10) val = rc_adc_read_raw(channel);
  else if (channel < 15) val = rc_adc_read_volt(channel-10);
  else if (channel == 15) val = rc_adc_dc_jack();
  else if (channel == 16) val = rc_adc_batt();

  double value = (val - offset) / scale;
  if( value > maxIn ) value = maxIn;
  if( value < minIn ) value = minIn;
  return value;
}


extern "C"{
  eeros::hal::ScalableInput<double> *createAnalogIn(std::string id, void* libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel, 
                                                    double scale, double offset, double rangeMin, double rangeMax, std::string unit) {
    return new bbblue::AnalogIn(id, libHandle, device, subDeviceNumber, channel, scale, offset, rangeMin, rangeMax, unit);
  }
}
