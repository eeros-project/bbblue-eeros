#include "../include/AnalogIn.hpp"
#include <iostream>
#include <unistd.h>

extern "C" {
#include <robotcontrol.h>
};

using namespace bbblue;
using namespace eeros::hal;

AnalogIn::AnalogIn(std::string id,  void* libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel, double scale,
                   double offset, double rangeMin, double rangeMax, std::string unit, bool getDelta) 
    : ScalableInput<double>(id, libHandle, scale, offset, rangeMin, rangeMax, unit), channel(channel) { 
  rc_adc_init();
  if (channel >= 30 && channel < 42 && first) {
    mpuConfig = rc_mpu_default_config();
    mpuConfig.enable_magnetometer = 1;
    mpuConfig.dmp_fetch_accel_gyro = 1;
    mpuConfig.dmp_sample_rate = 100;
    rc_mpu_initialize_dmp(&mpuData, mpuConfig);
//     rc_mpu_set_dmp_callback(&callback);
    first = false;
    usleep(100000); // wait for initialization
  }
  if (channel == 43) rc_bmp_init(BMP_OVERSAMPLE_4, BMP_FILTER_OFF);
}

AnalogIn::~AnalogIn() {
  rc_adc_cleanup();
  rc_mpu_power_off();
}

/**
 * Channel 20 and 21 can only be read with a maximum frequency of about 100Hz. Else the call will block indefinitely.
 */
double AnalogIn::get() {
  double val = 0;
  if (channel <= 3) val = rc_adc_read_raw(channel);
  else if (channel <= 13) val = rc_adc_read_volt(channel-10);
  else if (channel == 20) val = rc_adc_dc_jack();
  else if (channel == 21) val = rc_adc_batt();
  else if (channel >= 30 && channel <= 32) { // read the mpu just once for all channels 
//     if (accState & (1 << (channel - 30))) {
//       accState = 1 << (channel - 30);
// //       rc_mpu_read_accel(&mpuData);
//     } else accState |= 1 << (channel - 30);
    if (channel == 30) val = mpuData.raw_accel[0];
    else if (channel == 31) val = mpuData.raw_accel[1]; 
    else if (channel == 32) val = mpuData.raw_accel[2];
  } else if (channel >= 33 && channel <= 35) { // read the mpu just once for all channels 
//     if (gyroState & (1 << (channel - 30))) {
//       gyroState = 1 << (channel - 30);
// //       rc_mpu_read_gyro(&mpuData);
//     } else gyroState |= 1 << (channel - 30);
    if (channel == 33) val = mpuData.raw_gyro[0];
    else if (channel == 34) val = mpuData.raw_gyro[1];
    else if (channel == 35) val = mpuData.raw_gyro[2];
  } else if (channel >= 36 && channel <= 38) { // read the mpu just once for all channels 
//     if (magState & (1 << (channel - 30))) {
//       magState = 1 << (channel - 30);
// //       rc_mpu_read_mag(&mpuData);
//     } else magState |= 1 << (channel - 30);
    if (channel == 36) val = mpuData.mag[0];
    else if (channel == 37) val = mpuData.mag[1];
    else if (channel == 38) val = mpuData.mag[2];
  } else if (channel >= 39 && channel <= 42) { // read the mpu just once for all channels 
//     if (magState & (1 << (channel - 30))) {
//       magState = 1 << (channel - 30);
// //       rc_mpu_read_mag(&mpuData);
//     } else magState |= 1 << (channel - 30);
    if (channel == 39) val = mpuData.dmp_quat[0];
    else if (channel == 40) val = mpuData.dmp_quat[1];
    else if (channel == 41) val = mpuData.dmp_quat[2];
    else if (channel == 42) val = mpuData.dmp_quat[3];
  } else if (channel == 43) { 
    rc_bmp_read(&barData);
    val = barData.alt_m;
  }

  double value = (val - offset) / scale;
  if( value > maxIn ) value = maxIn;
  if( value < minIn ) value = minIn;
  return value;
}

rc_mpu_data_t AnalogIn::mpuData;
rc_mpu_config_t AnalogIn::mpuConfig;
// uint32_t AnalogIn::accState = 0;
// uint32_t AnalogIn::gyroState = 0;
// uint32_t AnalogIn::magState = 0;
bool AnalogIn::first = true;

extern "C"{
  eeros::hal::ScalableInput<double> *createAnalogIn(std::string id, void* libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel, 
                                                    double scale, double offset, double rangeMin, double rangeMax, std::string unit) {
    return new bbblue::AnalogIn(id, libHandle, device, subDeviceNumber, channel, scale, offset, rangeMin, rangeMax, unit);
  }

}
