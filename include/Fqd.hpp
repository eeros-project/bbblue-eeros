#ifndef BBBLUE_EEROS_FQD_HPP_
#define BBBLUE_EEROS_FQD_HPP_

#include <eeros/hal/ScalableInput.hpp>
#include <limits>
#include "BBBlueDevice.hpp"


namespace bbblue {
	class Fqd : public eeros::hal::ScalableInput<double> {
	public:
		Fqd(std::string id, void* libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel, 
		    double scale = 1, double offset = 0, 
		    double rangeMin = std::numeric_limits<double>::min(), double rangeMax = std::numeric_limits<double>::max(), 
		    std::string unit = "", bool getDelta = false);
		virtual double get();
		void reset();
		
	private:
//		flink_subdev* subdeviceHandle;
		uint32_t channel;
		uint16_t prevPos;
		double pos;
		bool getDelta;
	};
};

extern "C"{
	eeros::hal::ScalableInput<double> *createFqd(std::string id, void* libHandle, std::string device, uint32_t subDeviceNumber, 
						     uint32_t channel, double scale, double offset, 
						     double rangeMin, double rangeMax, std::string unit);
	void resetFqd(bbblue::Fqd *obj);
}

#endif /* BBBLUE_EEROS_FQD_HPP_ */
