#ifndef BBBLUE_EEROS_DIGOUT_HPP_
#define BBBLUE_EEROS_DIGOUT_HPP_

#include <gpiod.hpp>
#include <string>
#include <eeros/hal/Output.hpp>
#include <GPIO.hpp>
#include <variant>
namespace bbblue {

class DigOut : public eeros::hal::Output<bool> {
 public:
  DigOut(std::string id, void* libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel, bool inverted = false);
  virtual ~DigOut();
  virtual bool get();
  virtual void set(bool value);

 private:
  uint32_t channel;
  std::variant<LED, GPIO> output;
};

}

extern "C"{
  eeros::hal::Output<bool> *createDigOut(std::string id, void* libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel, bool inverted);
}

#endif /* BBBLUE_EEROS_DIGOUT_HPP_ */
