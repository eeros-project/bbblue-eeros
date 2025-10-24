#ifndef BBBLUE_EEROS_ANALOGOUT_HPP_
#define BBBLUE_EEROS_ANALOGOUT_HPP_

#include <cstdint>
#include <eeros/hal/ScalableOutput.hpp>
#include <fstream>
#include <limits>

#include <gpiod.hpp>
#include <optional>

namespace bbblue {

class AnalogOut : public eeros::hal::ScalableOutput<double> {
 public:
  AnalogOut(std::string id, void *libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel, double scale = 1, double offset = 0,
            double rangeMin = std::numeric_limits<double>::min(), double rangeMax = std::numeric_limits<double>::max(), std::string unit = "");
  virtual ~AnalogOut();
  virtual double get();
  virtual void set(double value);
  void freeSpin(int ch);
  void brake(int ch);

 private:
  struct MotorGPIO {
      gpiod::chip chipA;
      gpiod::chip chipB;
      gpiod::line_request dirA;
      gpiod::line_request dirB;
      gpiod::line::offset offsetA;
      gpiod::line::offset offsetB;
  };
  std::optional<MotorGPIO> motorPins;
  std::fstream dutyCycleFile;
  std::string unexportPath;
  uint32_t channel;
  unsigned int pwmChannel;
  double value;
  uint64_t periodNs;
};

}

extern "C"{
  eeros::hal::ScalableOutput<double> *createAnalogOut(std::string id, void* libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel,
                                                      double scale, double offset, double rangeMin, double rangeMax, std::string unit);
void freeSpin(bbblue::AnalogOut *obj, int ch);

void brake(bbblue::AnalogOut *obj, int ch);

}

#endif /* BBBLUE_EEROS_ANALOGOUT_HPP_ */
