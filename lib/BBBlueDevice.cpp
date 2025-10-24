#include "../include/BBBlueDevice.hpp"
#include <eeros/core/Fault.hpp>

using namespace bbblue;

BBBlueDevice::BBBlueDevice() : log(Logger::getLogger()), motorStandbyChip([](){
    return gpiod::chip("/dev/gpiochip3");
}()), motorStandbyGPIO([&](){
    auto lineNumber = motorStandbyChip.get_line_offset_from_name("MOT_STBY");
    auto settings = gpiod::line_settings{}.set_direction(gpiod::line::direction::OUTPUT);
    auto request = motorStandbyChip.prepare_request().set_consumer("BBBlue EEROS").add_line_settings(lineNumber, settings).do_request();
    request.set_value(lineNumber, gpiod::line::value::ACTIVE);
    return std::move(request);
}()) {}

BBBlueDevice::~BBBlueDevice() {
  motorStandbyGPIO.release();
  motorStandbyChip.close();
}

std::unique_ptr<BBBlueDevice> instance{new BBBlueDevice()};
