#include "../include/DigIn.hpp"
#include <gpiod.hpp>
#include <algorithm>
#include <array>
#include <Log.hpp>

using namespace bbblue;
using namespace eeros::hal;

static std::array<GPIOInfo, 8> gpioInfo = {{
    {0, "MODE_BTN"},
    {1, "PAUSE_BTN"},
    {117, "GPIO1_17"},
    {125, "GPIO1_25"},
    {31, "GPIO3_1"},
    {32, "GPIO3_2"},
    {317, "GPIO3_17"},
    {320, "GPIO3_20"}
}};

static GPIO initGPIO(uint32_t channel, bool inverted) {
    auto gpio = std::find_if(std::begin(gpioInfo), std::end(gpioInfo), [&](auto&& e){return e.channel == channel;});
    if(gpio == std::end(gpioInfo)) {
        std::cout << "invalid channel " << channel << '\n';
        std::exit(1);
    }
    // The 2 buttons (channel 1 and 2) are active low, so invert them (again) to make then work as expected
    inverted ^= (channel < 3);
    return requestGPIO(gpio->label, gpiod::line::direction::INPUT, inverted);
}

DigIn::DigIn(std::string id, void* libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel, bool inverted)
    : Input<bool>(id, libHandle), gpio(initGPIO(channel, inverted)) {}

DigIn::~DigIn() {
    gpio.request.release();
    gpio.chip.close();
}

bool DigIn::get() {
    return gpio.request.get_value(gpio.offset) == gpiod::line::value::ACTIVE;
}

extern "C" eeros::hal::Input<bool> *createDigIn(std::string id, void* libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel, bool inverted){
  return new bbblue::DigIn(id, libHandle, device, subDeviceNumber, channel, inverted);
}
