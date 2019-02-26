#include "../include/DigIn.hpp"

using namespace bbblue;
using namespace eeros::hal;

DigIn::DigIn(std::string id, void* libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel, bool inverted): 
				Input<bool>(id, libHandle) ,channel(channel), inverted(inverted) {
						if(channel > 1) {
							rc_gpio_export(channel);
							rc_gpio_set_dir(channel, INPUT_PIN);
						}
					}

bool DigIn::get() {
	bool value;
	if (channel == 0) value = rc_get_pause_button() == PRESSED;
	else if (channel == 1) value = rc_get_mode_button() == PRESSED;
	else{
		value = static_cast<bool>(rc_gpio_get_value(channel));
	}
	if(inverted) value = !value;
	return value;
}

extern "C" eeros::hal::Input<bool> *createDigIn(std::string id, void* libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel, bool inverted){
	return new bbblue::DigIn(id, libHandle, device, subDeviceNumber, channel, inverted);
}
