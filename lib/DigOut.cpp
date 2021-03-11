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
  rc_led_cleanup();
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
  else if (channel == 2) value = rc_led_get(RC_LED_USR0) != 0;
  else if (channel == 3) value = rc_led_get(RC_LED_USR1) != 0;
  else if (channel == 4) value = rc_led_get(RC_LED_USR2) != 0;
  else if (channel == 5) value = rc_led_get(RC_LED_USR3) != 0;
  else if (channel == 6) value = rc_led_get(RC_LED_BAT25) != 0;
  else if (channel == 7) value = rc_led_get(RC_LED_BAT50) != 0;
  else if (channel == 8) value = rc_led_get(RC_LED_BAT75) != 0;
  else if (channel == 9) value = rc_led_get(RC_LED_BAT100) != 0;
  else if (channel == 10) value = rc_led_get(RC_LED_WIFI) != 0;
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
  else if (channel == 2) rc_led_set(RC_LED_USR0, value);
  else if (channel == 3) rc_led_set(RC_LED_USR1, value);
  else if (channel == 4) rc_led_set(RC_LED_USR2, value);
  else if (channel == 5) rc_led_set(RC_LED_USR3, value);
  else if (channel == 6) rc_led_set(RC_LED_BAT25, value);
  else if (channel == 7) rc_led_set(RC_LED_BAT50, value);
  else if (channel == 8) rc_led_set(RC_LED_BAT75, value);
  else if (channel == 9) rc_led_set(RC_LED_BAT100, value);
  else if (channel == 10) rc_led_set(RC_LED_WIFI, value);
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
