#include "../include/DigIn.hpp"

using namespace bbblue;
using namespace eeros::hal;

DigIn::DigIn(std::string id, void* libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel, bool inverted)
    : Input<bool>(id, libHandle) ,channel(channel), inverted(inverted) {
  rc_button_init(RC_BTN_PIN_PAUSE, RC_BTN_POLARITY_NORM_HIGH, RC_BTN_DEBOUNCE_DEFAULT_US);
  rc_button_init(RC_BTN_PIN_MODE, RC_BTN_POLARITY_NORM_HIGH, RC_BTN_DEBOUNCE_DEFAULT_US);
  if (channel == 117) rc_gpio_init(1, 17, GPIOHANDLE_REQUEST_INPUT);
  if (channel == 125) rc_gpio_init(1, 25, GPIOHANDLE_REQUEST_INPUT);
  if (channel == 31) rc_gpio_init(3, 1, GPIOHANDLE_REQUEST_INPUT);
  if (channel == 32) rc_gpio_init(3, 2, GPIOHANDLE_REQUEST_INPUT);
  if (channel == 317) rc_gpio_init(3, 17, GPIOHANDLE_REQUEST_INPUT);
  if (channel == 320) rc_gpio_init(3, 20, GPIOHANDLE_REQUEST_INPUT);  
}

DigIn::~DigIn() {
  rc_button_cleanup();
  rc_gpio_cleanup(1, 17);
  rc_gpio_cleanup(1, 25);
  rc_gpio_cleanup(3, 1);
  rc_gpio_cleanup(3, 2);
  rc_gpio_cleanup(3, 17);
  rc_gpio_cleanup(3, 20);
}

bool DigIn::get() {
  bool value;
  if (channel == 0) value = rc_button_get_state(RC_BTN_PIN_PAUSE) == RC_BTN_STATE_PRESSED;
  else if (channel == 1) value = rc_button_get_state(RC_BTN_PIN_MODE) == RC_BTN_STATE_PRESSED;
  else if (channel == 117) value = rc_gpio_get_value(1, 17) != 0;
  else if (channel == 125) value = rc_gpio_get_value(1, 25) != 0;
  else if (channel == 31) value = rc_gpio_get_value(3, 1) != 0;
  else if (channel == 32) value = rc_gpio_get_value(3, 2) != 0;
  else if (channel == 317) value = rc_gpio_get_value(3, 17) != 0;
  else if (channel == 320) value = rc_gpio_get_value(3, 20) != 0;
  if (inverted) value = !value;
  return value;
}

extern "C" eeros::hal::Input<bool> *createDigIn(std::string id, void* libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel, bool inverted){
  return new bbblue::DigIn(id, libHandle, device, subDeviceNumber, channel, inverted);
}
