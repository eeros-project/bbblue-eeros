#include "../include/AnalogOut.hpp"
#include <chrono>

extern "C" {
#include <robotcontrol.h>
};

using namespace bbblue;
using namespace eeros::hal;

AnalogOut::AnalogOut(std::string id, void* libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel, 
                     double scale, double offset, double rangeMin, double rangeMax, std::string unit) 
    : ScalableOutput<double>(id, libHandle, scale, offset, rangeMin, rangeMax, unit), channel(channel) {
  if (channel < 10) rc_motor_init();
  else {
    rc_servo_init();
    rc_servo_power_rail_en(1);
  }
}

AnalogOut::~AnalogOut() {
  rc_motor_cleanup();
  rc_servo_cleanup();
}

double AnalogOut::get() {
  return value * scale + offset;
}

void AnalogOut::set(double voltage) {
  value = (voltage - offset) / scale;
  if (value > maxOut) value = maxOut;
  if (value < minOut) value = minOut;
  if (channel < 10) rc_motor_set(channel, (float)value);
  else {  // make sure that update of servo signal is not faster than 50Hz
    if (measure) {
      if (count == 0) start = std::chrono::high_resolution_clock::now();
      else if (count == 1) {
        double diff = std::chrono::duration<double, std::micro>(std::chrono::high_resolution_clock::now()-start).count();
        maxCount = 1000 * 40 / diff;  // 40ms update period -> 25Hz
        measure = false;
      }
    }
    count++;
    if (count >= maxCount) {
      rc_servo_send_pulse_normalized(channel - 10, value);
      count = 0;
    }
  }
}

void AnalogOut::freeSpin(int ch) {
  rc_motor_free_spin(ch);
}

void AnalogOut::brake(int ch) {
  rc_motor_brake(ch);
}

extern "C"{
  eeros::hal::ScalableOutput<double> *createAnalogOut(std::string id, void *libHandle, std::string device, uint32_t subDeviceNumber, 
                                                      uint32_t channel, double scale, double offset, double rangeMin, double rangeMax, std::string unit) {
    return new bbblue::AnalogOut(id, libHandle, device, subDeviceNumber, channel, scale, offset, rangeMin, rangeMax, unit);
  }

  void freeSpin(bbblue::AnalogOut *obj, int ch) {
    obj->freeSpin(ch);
  }
  
  void brake(bbblue::AnalogOut *obj, int ch) {
    obj->brake(ch);
  }
}


