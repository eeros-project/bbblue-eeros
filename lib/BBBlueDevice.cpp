#include "../include/BBBlueDevice.hpp"
#include <eeros/core/Fault.hpp>

using namespace bbblue;

BBBlueDevice::BBBlueDevice() : log(Logger::getLogger()), motorStandby(requestGPIO("MOT_STBY", gpiod::line::direction::OUTPUT)) {
    motorStandby.request.set_value(motorStandby.offset, gpiod::line::value::ACTIVE);
}

BBBlueDevice::~BBBlueDevice() {
  motorStandby.request.set_value(motorStandby.offset, gpiod::line::value::INACTIVE);
  motorStandby.request.release();
  motorStandby.chip.close();
}

std::unique_ptr<BBBlueDevice> instance{new BBBlueDevice()};
