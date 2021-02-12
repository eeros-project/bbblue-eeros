#include "../include/BBBlueDevice.hpp"
#include <eeros/core/Fault.hpp>

extern "C" {
#include <robotcontrol.h>
};

using namespace bbblue;

BBBlueDevice::BBBlueDevice() : log(Logger::getLogger()) {
  rc_model_t model = rc_model();
  if (model != MODEL_BB_BLUE) throw eeros::Fault("wrong platform for libbbblueeeros wrapper library");
  log.info() << "robot control initialized on BeagleBone blue"; 
  
  rc_disable_signal_handler();	// we use our own signal handler
  log.info() << "robot control: signal handling transfered to application";
}

BBBlueDevice::~BBBlueDevice() {
  log.info() << "robot control done";
}

std::unique_ptr<BBBlueDevice> instance{new BBBlueDevice()};
