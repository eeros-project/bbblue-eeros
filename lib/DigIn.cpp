#include "../include/DigIn.hpp"
#include <gpiod.hpp>
#include <algorithm>
#include <array>
#include <Log.hpp>

using namespace bbblue;
using namespace eeros::hal;

static std::array<GPIOInfo, 8> gpioInfo = {{
    {0, "/dev/gpiochip1", 4},
    {1, "/dev/gpiochip1", 5},
    {117, "/dev/gpiochip0", 17},
    {125, "/dev/gpiochip0", 25},
    {31, "/dev/gpiochip2", 1},
    {32, "/dev/gpiochip2", 2},
    {317, "/dev/gpiochip2", 17},
    {320, "/dev/gpiochip2", 20}
}};

static GPIO initGPIO(uint32_t channel, bool inverted) {
    auto gpio = std::find_if(std::begin(gpioInfo), std::end(gpioInfo), [&](auto&& e){return e.channel == channel;});
    if(gpio == std::end(gpioInfo)) {
        std::cout << "invalid channel " << channel << '\n';
        std::exit(1);
    }
    auto chip = gpiod::chip(gpio->path);
    // The 2 buttons (channel 1 and 2) are active low, so invert them (again) to make then work as expected
    inverted ^= (channel < 3);
    auto settings = gpiod::line_settings{}.set_direction(gpiod::line::direction::INPUT).set_active_low(inverted);
    auto request = chip.prepare_request().set_consumer("BBBlue EEROS").add_line_settings(gpio->offset, settings).do_request();
    return GPIO{std::move(chip), std::move(request), gpio->offset};
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
