#include "../include/DigOut.hpp"

using namespace bbblue;
using namespace eeros::hal;

DigOut::DigOut(std::string id, void *libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel, bool inverted) 
    : Output<bool>(id, libHandle), channel(channel), inverted(inverted) {
  if (channel == 117) rc_gpio_init(1, 17, GPIOHANDLE_REQUEST_OUTPUT);
  if (channel == 125) rc_gpio_init(1, 25, GPIOHANDLE_REQUEST_OUTPUT);
  if (channel == 31) rc_gpio_init(3, 1, GPIOHANDLE_REQUEST_OUTPUT);
  if (channel == 32) rc_gpio_init(3, 2, GPIOHANDLE_REQUEST_OUTPUT);
  if (channel == 317) rc_gpio_init(3, 17, GPIOHANDLE_REQUEST_OUTPUT);
  if (channel == 320) rc_gpio_init(3, 20, GPIOHANDLE_REQUEST_OUTPUT);  
}

DigOut::~DigOut() {
  rc_gpio_cleanup(1, 17);
  rc_gpio_cleanup(1, 25);
  rc_gpio_cleanup(3, 1);
  rc_gpio_cleanup(3, 2);
  rc_gpio_cleanup(3, 17);
  rc_gpio_cleanup(3, 20);
}

bool DigOut::get(){
  bool value = false;
  if (channel == 0) value = rc_led_get(RC_LED_GREEN) != 0;
  else if (channel == 1) value = rc_led_get(RC_LED_RED) != 0;
  else if (channel == 117) value = rc_gpio_get_value(1, 17) != 0;
  else if (channel == 125) value = rc_gpio_get_value(1, 25) != 0;
  else if (channel == 31) value = rc_gpio_get_value(3, 1) != 0;
  else if (channel == 32) value = rc_gpio_get_value(3, 2) != 0;
  else if (channel == 317) value = rc_gpio_get_value(3, 17) != 0;
  else if (channel == 320) value = rc_gpio_get_value(3, 20) != 0;
  if (inverted) value = !value;
  return value;
}

void DigOut::set(bool value){
  if (inverted) value = !value;
  if (channel == 0) rc_led_set(RC_LED_GREEN, value);
  else if (channel == 1) rc_led_set(RC_LED_RED, value);
  else if (channel == 117) rc_gpio_set_value(1, 17, value);
  else if (channel == 125) rc_gpio_set_value(1, 25, value);
  else if (channel == 31) rc_gpio_set_value(3, 1, value);
  else if (channel == 32) rc_gpio_set_value(3, 2, value);
  else if (channel == 317) rc_gpio_set_value(3, 17, value);
  else if (channel == 320) rc_gpio_set_value(3, 20, value);
}

extern "C" eeros::hal::Output<bool> *createDigOut(std::string id, void* libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel, bool inverted){
  return new bbblue::DigOut(id, libHandle, device, subDeviceNumber, channel, inverted);
}
