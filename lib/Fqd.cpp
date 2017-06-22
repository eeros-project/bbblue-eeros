#include "../include/Fqd.hpp"

extern "C" {
#include "../../Robotics_Cape_Installer/libraries/rc_usefulincludes.h"
#include "../../Robotics_Cape_Installer/libraries/roboticscape.h"
};

using namespace bbblue;
using namespace eeros::hal;

Fqd::Fqd(std::string id,  void* libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel, double scale,
		double offset, double rangeMin, double rangeMax, std::string unit, bool getDelta) : 
	ScalableInput<double>(id, libHandle, scale, offset, rangeMin, rangeMax, unit), channel(channel), prevPos(0), getDelta(getDelta) {
	reset();
}

double Fqd::get() {
	uint32_t data = rc_get_encoder_pos(channel);
	int16_t newPos = static_cast<uint16_t>(data);
	int16_t deltaPos = newPos - prevPos;
	prevPos = newPos;
	double delta = deltaPos / scale + offset;
	pos += delta;
	
	if (getDelta)
		return delta;
	else
		return pos;
}

void Fqd::reset() {
	rc_set_encoder_pos(channel, 0);
	pos = 0;
	prevPos = 0;
}

extern "C"{
	eeros::hal::ScalableInput<double> *createFqd(std::string id, void* libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel, 
						     double scale, double offset, double rangeMin, double rangeMax, std::string unit){
		return new bbblue::Fqd(id, libHandle, device, subDeviceNumber, channel, scale, offset, rangeMin, rangeMax, unit);
	}
	
	void resetFqd(bbblue::Fqd *obj){
		obj->reset();
	}
}
