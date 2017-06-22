#ifndef BBBLUE_EEROS_DIGOUT_HPP_
#define BBBLUE_EEROS_DIGOUT_HPP_

#include <string>
#include <eeros/hal/Output.hpp>
#include "BBBlueDevice.hpp"

extern "C" {
#include "../../Robotics_Cape_Installer/libraries/rc_usefulincludes.h"
#include "../../Robotics_Cape_Installer/libraries/roboticscape.h"
};

namespace bbblue {
	class DigOut : public eeros::hal::Output<bool> {
	public:
		DigOut(std::string id, void* libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel, bool inverted = false);
		virtual bool get();
		virtual void set(bool value);
	private:
		rc_led_t channel;
		bool inverted;
	};
};

extern "C"{
	eeros::hal::Output<bool> *createDigOut(std::string id, void* libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel, bool inverted);
}

#endif /* BBBLUE_EEROS_DIGOUT_HPP_ */