#ifndef BBBLUE_EEROS_DEVICE_HPP_
#define BBBLUE_EEROS_DEVICE_HPP_

#include <eeros/logger/Logger.hpp>
#include <gpiod.hpp>


namespace bbblue {
 
using namespace eeros::logger;

class BBBlueDevice {
 public:
  BBBlueDevice();
  virtual ~BBBlueDevice();
  
 private:
  Logger log;
  gpiod::chip motorStandbyChip;
  gpiod::line_request motorStandbyGPIO;
};

}

#endif /* BBBLUE_EEROS_DEVICE_HPP_ */
