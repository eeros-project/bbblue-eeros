#ifndef BBBLUE_EEROS_ANALOGOUT_HPP_
#define BBBLUE_EEROS_ANALOGOUT_HPP_

#include <eeros/hal/ScalableOutput.hpp>
#include <limits>
#include "BBBlueDevice.hpp"

namespace bbblue {
  
class AnalogOut : public eeros::hal::ScalableOutput<double> {
 public:
  AnalogOut(std::string id, void *libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel, double scale = 1, double offset = 0, 
            double rangeMin = std::numeric_limits<double>::min(), double rangeMax = std::numeric_limits<double>::max(), std::string unit = "");
  virtual double get();
  virtual void set(double value);

 private:
  uint32_t channel;
  double value;
};

};

extern "C"{
  eeros::hal::ScalableOutput<double> *createAnalogOut(std::string id, void* libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel, 
                                                      double scale, double offset, double rangeMin, double rangeMax, std::string unit);
}

#endif /* BBBLUE_EEROS_ANALOGOUT_HPP_ */