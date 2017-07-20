#include "../include/BBBlueDevice.hpp"
#include <eeros/core/Fault.hpp>
#include <iostream>

extern "C" {
#include <rc_usefulincludes.h>
#include <roboticscape.h>
};

using namespace bbblue;

BBBlueDevice::BBBlueDevice() {
	if(rc_initialize()){
		throw eeros::Fault("Can't initialize robotics cape");
	}
	std::cout << "robotics cape initialized" << std::endl;
	
	rc_disable_signal_handler();	// we use our own signal handler
	std::cout << "signal handling transfered to application" << std::endl;
}

BBBlueDevice::~BBBlueDevice() {
	rc_cleanup();
}

BBBlueDevice* BBBlueDevice::instance = new BBBlueDevice();
