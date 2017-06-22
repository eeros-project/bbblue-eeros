#include "../include/DigIn.hpp"

using namespace bbblue;
using namespace eeros::hal;

DigIn::DigIn(std::string id, void* libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel, bool inverted): 
				Input<bool>(id, libHandle) ,channel(channel), inverted(inverted) { }

bool DigIn::get() {
	bool value;
	if (channel == 0) value = rc_get_pause_button() == PRESSED;
	else value = rc_get_mode_button() == PRESSED;
	if(inverted) value = !value;
	return value;
}

extern "C" eeros::hal::Input<bool> *createDigIn(std::string id, void* libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel, bool inverted){
	return new bbblue::DigIn(id, libHandle, device, subDeviceNumber, channel, inverted);
}