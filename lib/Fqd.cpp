#include "../include/Fqd.hpp"

extern "C" {
#include <robotcontrol.h>
};

using namespace bbblue;
using namespace eeros::hal;

Fqd::Fqd(std::string id,  void* libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel, double scale,
    double offset, double rangeMin, double rangeMax, std::string unit, bool getDelta) 
    : ScalableInput<double>(id, libHandle, scale, offset, rangeMin, rangeMax, unit), channel(channel), prevPos(0) {
  rc_encoder_init();
  reset();
}

Fqd::~Fqd() {
  rc_encoder_cleanup();
}


double Fqd::get() {
  int32_t actPos = rc_encoder_read(channel);
  int32_t deltaPos = actPos - prevPos;
  prevPos = actPos;
  pos += deltaPos / scale + offset;
  return pos;
}

void Fqd::reset() {
  rc_encoder_write(channel, 0);
  pos = 0;
  prevPos = 0;
}

void Fqd::setPos(double position){
  pos = position;
  prevPos = pos*scale+offset;
  rc_encoder_write(channel, pos*scale+offset);
}

extern "C"{
  eeros::hal::ScalableInput<double> *createFqd(std::string id, void* libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel, 
      double scale, double offset, double rangeMin, double rangeMax, std::string unit) {
    return new bbblue::Fqd(id, libHandle, device, subDeviceNumber, channel, scale, offset, rangeMin, rangeMax, unit);
  }
  
  void resetFqd(bbblue::Fqd *obj){
    obj->reset();
  }

  void setFqdPos(bbblue::Fqd *obj, double position){
    obj->setPos(position);
  }
}
