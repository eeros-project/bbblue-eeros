#ifndef BBBLUE_EEROS_ANALOGIN_HPP_
#define BBBLUE_EEROS_ANALOGIN_HPP_

#include "Sysfs.hpp"
#include <eeros/hal/ScalableInput.hpp>
#include <limits>

namespace bbblue {
  
class AnalogIn : public eeros::hal::ScalableInput<double> {
 public:
  AnalogIn(std::string id, void* libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel, 
           double scale = 1, double offset = 0, double rangeMin = std::numeric_limits<double>::min(), 
           double rangeMax = std::numeric_limits<double>::max(), std::string unit = "");
  virtual ~AnalogIn();
  virtual double get();

 private:
  int32_t channel;
  SysfsFile file;
};

}

extern "C"{
  eeros::hal::ScalableInput<double> *createAnalogIn(std::string id, void* libHandle, std::string device, uint32_t subDeviceNumber, 
                                                    uint32_t channel, double scale, double offset, 
                                                    double rangeMin, double rangeMax, std::string unit);

}

#endif /* BBBLUE_EEROS_ANALOGIN_HPP_ */
