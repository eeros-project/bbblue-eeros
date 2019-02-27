#include "../include/DigOut.hpp"

using namespace bbblue;
using namespace eeros::hal;

DigOut::DigOut(std::string id, void *libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel, bool inverted) : 
	Output<bool>(id, libHandle),
	inverted(inverted) {
		this->channel = channel;

		if(channel > 1){
			rc_gpio_export(channel);
			rc_gpio_set_dir(channel, OUTPUT_PIN);
		}
}

bool DigOut::get(){
	bool value = false;
	if(channel == 0)
		value = static_cast<bool>(rc_get_led(GREEN));
	else if(channel == 1)
		value = static_cast<bool>(rc_get_led(RED));
	else value = static_cast<bool>(rc_gpio_get_value(channel));
	if(inverted) value = !value;
	return value;
}

void DigOut::set(bool value){
	if(inverted) value = !value;
	if(channel == 0) rc_set_led(GREEN, value);
	else if(channel == 1)rc_set_led(RED, value);
	else rc_gpio_set_value(channel, value);
}

extern "C" eeros::hal::Output<bool> *createDigOut(std::string id, void* libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel, bool inverted){
	return new bbblue::DigOut(id, libHandle, device, subDeviceNumber, channel, inverted);
}
