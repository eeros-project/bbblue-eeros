#include "../include/DigOut.hpp"

using namespace bbblue;
using namespace eeros::hal;

DigOut::DigOut(std::string id, void *libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel, bool inverted) : 
	Output<bool>(id, libHandle),
	inverted(inverted) {
		  if (channel == 0) this->channel = GREEN;
		  else this->channel = RED;
}

bool DigOut::get(){
	bool value = static_cast<bool>(rc_get_led(channel));
	if(inverted) value = !value;
	return value;
}

void DigOut::set(bool value){
	if(inverted) value = !value;
	rc_set_led(channel, value);
}

extern "C" eeros::hal::Output<bool> *createDigOut(std::string id, void* libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel, bool inverted){
	return new bbblue::DigOut(id, libHandle, device, subDeviceNumber, channel, inverted);
}