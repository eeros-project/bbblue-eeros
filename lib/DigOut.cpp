#include "../include/DigOut.hpp"
#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <gpiod.hpp>
#include <iterator>
#include <array>
#include <unistd.h>
#include <variant>
#include <Log.hpp>

using namespace bbblue;
using namespace eeros::hal;
using Value = gpiod::line::value;



static std::array<LEDInfo, 11> ledInfo = {{
    {0, "/sys/class/leds/green"},
    {1, "/sys/class/leds/red"},
    {2, "/sys/class/leds/beaglebone:green:usr0"},
    {3, "/sys/class/leds/beaglebone:green:usr1"},
    {4, "/sys/class/leds/beaglebone:green:usr2"},
    {5, "/sys/class/leds/beaglebone:green:usr3"},
    {6, "/sys/class/leds/bat25"},
    {7, "/sys/class/leds/bat50"},
    {8, "/sys/class/leds/bat75"},
    {9, "/sys/class/leds/bat100"},
    {10, "/sys/class/leds/wifi"},
}};

static std::array<GPIOInfo, 6> gpioInfo = {{
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
        std::cout << "inalid channel " << channel << '\n';
        std::exit(1);
    }
    auto chip = gpiod::chip(gpio->path);
    auto settings = gpiod::line_settings{}.set_direction(gpiod::line::direction::OUTPUT).set_active_low(inverted);
    auto request = chip.prepare_request().set_consumer("BBBlue EEROS").add_line_settings(gpio->offset, settings).do_request();
    return GPIO{std::move(chip), std::move(request), gpio->offset};
}

static LED initLED(uint32_t channel, bool inverted) {
    auto info = std::find_if(std::begin(ledInfo), std::end(ledInfo), [&](auto&& led) {return led.channel == channel;});
    if(info == std::end(ledInfo)) {
        std::cout << "inalid channel " << channel << '\n';
        std::exit(1);
    }
    auto triggerPath = info->path / "trigger";
    std::fstream triggerFile(triggerPath);
    std::string contents = " ";
    while(contents[0] != '[')
        triggerFile >> contents;
    auto trigger = contents.substr(1, contents.size() - 2);
    debug() << "trigger: " << trigger << '\n';
    triggerFile << "none\n";

    return LED{std::fstream{info->path / "brightness"}, inverted, triggerPath, trigger};
}

static std::variant<LED, GPIO> initOutput(uint32_t channel, bool inverted) {
    if(channel < ledInfo.size()) return {initLED(channel, inverted)};
    else return {initGPIO(channel, inverted)};
}

DigOut::DigOut(std::string id, void *libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel, bool inverted)
    : Output<bool>(id, libHandle), channel(channel), output(initOutput(channel, inverted)) {}

static void cleanup(LED& led) {
    led.file.close();
    debug() << "setting led " << led.triggerPath << " back to " << led.originalTrigger << '\n';
    std::fstream triggerFile(led.triggerPath);
    triggerFile << led.originalTrigger << '\n';
}
static void cleanup(GPIO& gpio) {
    gpio.request.release();
    gpio.chip.close();
}

DigOut::~DigOut() {
    std::visit([&](auto&& out){cleanup(out);}, output);
}

static bool get_impl(LED& led) {
    uint32_t brightness = 0;
    led.file >> brightness;
    return brightness > 0;
}

static bool get_impl(GPIO& gpio) {
    return gpio.request.get_value(gpio.offset) == Value::ACTIVE;
}

bool DigOut::get(){
  return std::visit([&](auto&& out){return get_impl(out);}, output);
}

static void set_impl(LED& led, bool value) {
    led.file.seekp(0);
    led.file << (value ? "1\n" : "0\n");
}

static void set_impl(GPIO& gpio, bool value) {
    gpio.request.set_value(gpio.offset, value ? Value::ACTIVE : Value::INACTIVE);
}

void DigOut::set(bool value){
    std::visit([&](auto&& out){set_impl(out, value);}, output);
}

extern "C" eeros::hal::Output<bool> *createDigOut(std::string id, void* libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel, bool inverted){
  return new bbblue::DigOut(id, libHandle, device, subDeviceNumber, channel, inverted);
}
