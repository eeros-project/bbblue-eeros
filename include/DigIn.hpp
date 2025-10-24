#ifndef BBBLUE_EEROS_DIGIN_HPP_
#define BBBLUE_EEROS_DIGIN_HPP_

#include <string>
#include <eeros/hal/Input.hpp>

#include <GPIO.hpp>

namespace bbblue {

class DigIn : public eeros::hal::Input<bool> {
 public:
  DigIn(std::string id, void* libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel, bool inverted = false);
  virtual ~DigIn();
  virtual bool get();

 private:
 GPIO gpio;
};

}

extern "C"{
  eeros::hal::Input<bool> *createDigIn(std::string id, void* libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel, bool inverted);
}

#endif /* BBBLUE_EEROS_DIGIN_HPP_ */
