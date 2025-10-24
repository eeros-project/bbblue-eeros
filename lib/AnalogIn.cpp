#include "../include/AnalogIn.hpp"
#include <cstdint>
#include <dirent.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <Log.hpp>

using namespace bbblue;
using namespace eeros::hal;

constexpr const char* channelToDeviceName(uint32_t channel) {
    if(channel <= 21) return "TI-am335x-adc.0.auto";
    else if (channel >= 30 && channel <= 42) return "mpu9250";
    else if(channel == 43) return "bmp280";
    else return "invalid";
}

constexpr const char* channelToIIOChannelName(uint32_t channel) {
    switch(channel) {
        case 0: return "in_voltage0_raw";
        case 1: return "in_voltage1_raw";
        case 2: return "in_voltage2_raw";
        case 3: return "in_voltage3_raw";
        case 10: return "in_voltage0_raw";
        case 11: return "in_voltage1_raw";
        case 12: return "in_voltage2_raw";
        case 13: return "in_voltage3_raw";
        case 20: return "in_voltage5_raw";
        case 21: return "in_voltage6_raw";
        case 30: return "in_accel_x_raw";
        case 31: return "in_accel_y_raw";
        case 32: return "in_accel_z_raw";
        case 33: return "in_anglvel_x_raw";
        case 34: return "in_anglvel_y_raw";
        case 35: return "in_anglvel_z_raw";
        case 36: return "in_magn_x_raw";
        case 37: return "in_magn_y_raw";
        case 38: return "in_magn_z_raw";
        // TODO: how do we handle quaternions (channels 39 - 42)??
        case 43: return "in_pressure_input";
        default:
            std::cout << "unknown channel: " << channel << '\n';
            return "invalid";
    }
}

constexpr const char* channelScaleName(uint32_t channel) {
    if(channel <= 21) return "in_voltage_scale";
    switch (channel) {
        case 30: return "in_accel_scale";
        case 31: return "in_accel_scale";
        case 32: return "in_accel_scale";
        case 33: return "in_anglvel_scale";
        case 34: return "in_anglvel_scale";
        case 35: return "in_anglvel_scale";
        case 36: return "in_magn_x_scale";
        case 37: return "in_magn_y_scale";
        case 38: return "in_magn_z_scale";
        default: return "invalid";
    }
}

constexpr double extraChannelScale(uint32_t channel) {
    switch (channel) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 10:
        case 11:
        case 12:
        case 13: return 1000.0;
        case 20:
        case 21: return 1000.0/11;
        default: return 1.0;
    }
}

AnalogIn::AnalogIn(std::string id,  void* libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel, double scale,
                   double offset, double rangeMin, double rangeMax, std::string unit)
    : ScalableInput<double>(id, libHandle, scale, offset, rangeMin, rangeMax, unit), channel(channel) {
        debug() << "AnalogIn(id = " << id << ", channel = " << std::dec << channel << ")\n";
        std::string iioBasePath{"/sys/bus/iio/devices/"};
        auto iioDeviceDirectory = opendir(iioBasePath.c_str());
        if(iioDeviceDirectory == nullptr) {
            std::cout << "iioDeviceDirectory is NULL!\n";
            std::exit(1);
        }
        std::string devicePath;
        while(true) {
            auto dir = readdir(iioDeviceDirectory);
            if(dir == nullptr) break;
            std::string_view dirname{dir->d_name};
            if(dirname == "." || dirname == "..") continue;
            debug() << "found IIO device " << dirname << '\n';
            devicePath = iioBasePath + dirname.data();
            std::string deviceNamePath = devicePath + "/name";
            std::string name;
            std::ifstream{deviceNamePath} >> name;
            if(name == channelToDeviceName(channel)) {
                break;
            }
        }
        closedir(iioDeviceDirectory);


        if(channel != 43) {
            auto scaleFilePath = devicePath + "/" + channelScaleName(channel);
            auto scaleFile = SysfsFile(scaleFilePath);
            auto iioScale = scaleFile.readDecimal();
            debug() << "scale = " << scale << '\n';
            scale /= iioScale;
            debug() << "iioScale = " << iioScale << ", adjusted scale = " << scale << '\n';
        }
        scale *= extraChannelScale(channel);
        debug() << "extraChannelScale = " << extraChannelScale(channel) << ", adjusted scale = " << scale << '\n';
        debug() << "channel " << channel << " scale = " << scale << '\n';
        this->scale = scale;
        auto filePath = devicePath + "/" + channelToIIOChannelName(channel);
        file = SysfsFile(filePath);
    }

AnalogIn::~AnalogIn() {}

/**
 * Channel 20 and 21 can only be read with a maximum frequency of about 100Hz. Else the call will block indefinitely.
 */
double AnalogIn::get() {
  double val = file.readDecimal();
  double value = (val - offset) / scale;
  if( value > maxIn ) value = maxIn;
  if( value < minIn ) value = minIn;
  return value;
}

extern "C"{
  eeros::hal::ScalableInput<double> *createAnalogIn(std::string id, void* libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel,
                                                    double scale, double offset, double rangeMin, double rangeMax, std::string unit) {
    return new bbblue::AnalogIn(id, libHandle, device, subDeviceNumber, channel, scale, offset, rangeMin, rangeMax, unit);
  }

}
