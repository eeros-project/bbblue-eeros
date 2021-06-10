#ifndef BBBLUE_EEROS_ANALOGIN_HPP_
#define BBBLUE_EEROS_ANALOGIN_HPP_

#include <eeros/hal/ScalableInput.hpp>
#include <limits>
#include <chrono>

extern "C" {
#include <robotcontrol.h>
};

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
  static rc_mpu_config_t mpuConfig;
  static rc_mpu_data_t mpuData;
  static rc_bmp_data_t bmpData;
  static bool first;
  bool measure = true;
  uint32_t count = 0, maxCount = 1000;
  std::chrono::time_point<std::chrono::high_resolution_clock> start;
  static void dmpHandler(void);
};

}

extern "C"{
  eeros::hal::ScalableInput<double> *createAnalogIn(std::string id, void* libHandle, std::string device, uint32_t subDeviceNumber, 
                                                    uint32_t channel, double scale, double offset, 
                                                    double rangeMin, double rangeMax, std::string unit);

}

#endif /* BBBLUE_EEROS_ANALOGIN_HPP_ */
