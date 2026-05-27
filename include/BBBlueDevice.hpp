#ifndef BBBLUE_EEROS_DEVICE_HPP_
#define BBBLUE_EEROS_DEVICE_HPP_

#include "GPIO.hpp"
#include <eeros/logger/Logger.hpp>


namespace bbblue {

using namespace eeros::logger;

class BBBlueDevice {
 public:
  BBBlueDevice();
  virtual ~BBBlueDevice();

 private:
  Logger log;
  GPIO motorStandby;
};

}

#endif /* BBBLUE_EEROS_DEVICE_HPP_ */
