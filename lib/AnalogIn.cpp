#include "../include/AnalogIn.hpp"
#include <iostream>
#include <unistd.h>

extern "C" {
#include <robotcontrol.h>
};

using namespace bbblue;
using namespace eeros::hal;

AnalogIn::AnalogIn(std::string id,  void* libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel, double scale,
                   double offset, double rangeMin, double rangeMax, std::string unit) 
    : ScalableInput<double>(id, libHandle, scale, offset, rangeMin, rangeMax, unit), channel(channel) { 
  rc_adc_init();
  if (channel >= 30 && channel <= 42 && first) {
    // reading the imu and barometer over the same i2c is tricky, make sure that 
    // bmp is initialized even if it is not used, imu read must be finished when 
    // accessing the barometer 
    while (rc_bmp_init(BMP_OVERSAMPLE_16, BMP_FILTER_16)) std::cout << "init bmp again" << std::endl;
    mpuConfig = rc_mpu_default_config();
    mpuConfig.enable_magnetometer = 1;
    mpuConfig.dmp_fetch_accel_gyro = 1;
    mpuConfig.dmp_sample_rate = 200;
    mpuConfig.show_warnings = 0;
    rc_mpu_initialize_dmp(&mpuData, mpuConfig);
    first = false;
    usleep(100000); // wait for dmp to settle then start callback
    rc_mpu_set_dmp_callback(AnalogIn::dmpHandler);
  }
}

AnalogIn::~AnalogIn() {
  rc_adc_cleanup();
  rc_mpu_power_off();
  rc_bmp_power_off();
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
    if (channel == 30) val = mpuData.raw_accel[0];
    else if (channel == 31) val = mpuData.raw_accel[1]; 
    else if (channel == 32) val = mpuData.raw_accel[2];
  } else if (channel >= 33 && channel <= 35) { // read the mpu just once for all channels 
    if (channel == 33) val = mpuData.raw_gyro[0];
    else if (channel == 34) val = mpuData.raw_gyro[1];
    else if (channel == 35) val = mpuData.raw_gyro[2];
  } else if (channel >= 36 && channel <= 38) { // read the mpu just once for all channels 
    if (channel == 36) val = mpuData.mag[0];
    else if (channel == 37) val = mpuData.mag[1];
    else if (channel == 38) val = mpuData.mag[2];
  } else if (channel >= 39 && channel <= 42) { // read the mpu just once for all channels 
    if (channel == 39) val = mpuData.dmp_quat[0];
    else if (channel == 40) val = mpuData.dmp_quat[1];
    else if (channel == 41) val = mpuData.dmp_quat[2];
    else if (channel == 42) val = mpuData.dmp_quat[3];
  } else if (channel == 43) val = bmpData.alt_m;

  double value = (val - offset) / scale;
  if( value > maxIn ) value = maxIn;
  if( value < minIn ) value = minIn;
  return value;
}

void AnalogIn::dmpHandler(void) {
  static int count = 0;
  count++;
  if (count >= 10) { // check if we need to sample BMP 
    if(rc_bmp_read(&bmpData)) return;
    count = 0;
  }
}

rc_mpu_data_t AnalogIn::mpuData;
rc_mpu_config_t AnalogIn::mpuConfig;
rc_bmp_data_t AnalogIn::bmpData;
bool AnalogIn::first = true;

extern "C"{
  eeros::hal::ScalableInput<double> *createAnalogIn(std::string id, void* libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel, 
                                                    double scale, double offset, double rangeMin, double rangeMax, std::string unit) {
    return new bbblue::AnalogIn(id, libHandle, device, subDeviceNumber, channel, scale, offset, rangeMin, rangeMax, unit);
  }

}
