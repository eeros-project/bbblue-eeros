#include "../include/AnalogOut.hpp"

#include <Helpers.hpp>
#include <algorithm>
#include <cstdint>
#include <dirent.h>
#include <array>
#include <filesystem>
#include <fstream>
#include <gpiod.hpp>
#include <sstream>
#include <tuple>
#include <utility>
#include <Log.hpp>

using namespace bbblue;
using namespace eeros::hal;

struct MDir {
    const char* chip;
    const char* name;
};

struct MotorInfo {
    const char* pwmChip;
    unsigned int pwmChannel;
    MDir dirA;
    MDir dirB;
};

static const std::array<MotorInfo, 4> motorInfo{
    {{"ehrpwm1", 0, {"gpiochip1", "MDIR_1A"}, {"gpiochip3", "MDIR_1B"}},
    {"ehrpwm1", 1, {"gpiochip0", "MDIR_2A"}, {"gpiochip3", "MDIR_2B"}},
    {"ehrpwm2", 0, {"gpiochip1", "MDIR_3A"}, {"gpiochip1", "MDIR_3B"}},
    {"ehrpwm2", 1, {"gpiochip1", "MDIR_4A"}, {"gpiochip1", "MDIR_4B"}},}
};

constexpr double adjustScale(uint32_t channel, double scale) {
    switch (channel) {
        case 2:
        case 3: return -scale;
        default: return scale;
    }
}

AnalogOut::AnalogOut(std::string id, void* libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel,
                     double scale, double offset, double rangeMin, double rangeMax, std::string unit)
    : ScalableOutput<double>(id, libHandle, adjustScale(channel, scale), offset, rangeMin, rangeMax, unit), channel(channel) {
    const char* pwmDevice;
    // channel starts at 1, not 0
    if(channel-1 < motorInfo.size()) pwmDevice = motorInfo[channel-1].pwmChip;
    else pwmDevice = "pru1";
    auto wantedHandle = deviceTreeSymbolToPHandle(pwmDevice);
    std::string pwmDeviceName;
    auto pwmDeviceDirectory = opendir("/sys/class/pwm");
    while(true) {
        auto dir = readdir(pwmDeviceDirectory);
        if(dir == nullptr) break;
        std::string_view dirname{dir->d_name};
        if(dirname == "." || dirname == "..") continue;
        std::string pHandlePath{"/sys/class/pwm/"};
        pHandlePath.append(dirname);
        pHandlePath.append("/device/of_node/phandle");
        auto handle = readPHandle(pHandlePath);
        if(handle == wantedHandle) {
            pwmDeviceName = dirname;
            break;
        }
    }
    closedir(pwmDeviceDirectory);

    if(channel-1 < motorInfo.size()) {
        auto motor = motorInfo[channel-1];
        pwmChannel = motor.pwmChannel;
        auto requestGPIO = [&](auto info) {
            std::filesystem::path chipPath = "/dev";
            chipPath.append(info.chip);
            gpiod::chip chip{chipPath};
            auto lineNumber = chip.get_line_offset_from_name(info.name);
            auto settings = gpiod::line_settings{}.set_direction(gpiod::line::direction::OUTPUT);
            auto request = chip.prepare_request().set_consumer("BBBlue EEROS").add_line_settings(lineNumber, settings).do_request();
            return std::move(std::tuple(std::move(chip), std::move(request), lineNumber));
        };
        auto a = requestGPIO(motor.dirA);
        auto b = requestGPIO(motor.dirB);
        motorPins = MotorGPIO{std::move(std::get<0>(a)), std::move(std::get<0>(b)), std::move(std::get<1>(a)), std::move(std::get<1>(b)), std::get<2>(a), std::get<2>(b)};
    } else {
        motorPins = {};
        pwmChannel = channel - 10;
    }

    std::string basePath{"/sys/class/pwm/"};
    basePath.append(pwmDeviceName);
    auto exportPath = basePath + "/export";
    unexportPath = basePath + "/unexport";
    std::ofstream{exportPath} << pwmChannel << '\n';
    auto pwmChannelName = (std::ostringstream{} << "/pwm" << pwmChannel).str();
    auto enablePath = basePath + pwmChannelName + "/enable";
    auto dutyCyclePath = basePath + pwmChannelName + "/duty_cycle";
    auto periodPath = basePath + pwmChannelName + "/period";

    std::fstream periodFile{periodPath};
    periodFile << "100000\n"; // 100us -> 10kHz
    periodFile.seekp(0);
    // make sure we get the actual period the HW can actually provide
    periodFile >> periodNs;
    debug() << "periodNs: " << periodNs << '\n';
    dutyCycleFile = std::fstream(dutyCyclePath);
    dutyCycleFile << "0\n";
    std::fstream enableFile{enablePath};
    enableFile << "1\n";
}

AnalogOut::~AnalogOut() {
    if(motorPins) {
        motorPins->dirA.release();
        motorPins->dirB.release();
        motorPins->chipA.close();
        motorPins->chipB.close();
    }

    std::ofstream{unexportPath} << pwmChannel << '\n';
}

double AnalogOut::get() {
  return value * scale + offset;
}

void AnalogOut::set(double voltage) {
  value = (voltage - offset) / scale;
  if (value > maxOut) value = maxOut;
  if (value < minOut) value = minOut;
    if(motorPins.has_value()) {
        auto& dirA = motorPins->dirA;
        auto& dirB = motorPins->dirB;
        auto offA = motorPins->offsetA;
        auto offB = motorPins->offsetB;
        if(value >= 0) {
            dirB.set_value(offB, gpiod::line::value::INACTIVE);
            dirA.set_value(offA, gpiod::line::value::ACTIVE);
        } else {
            dirA.set_value(offA, gpiod::line::value::INACTIVE);
            dirB.set_value(offB, gpiod::line::value::ACTIVE);
            value = -value;
        }
    }
    value = std::max(0.0, std::min(value, 1.0));
    uint64_t highTime = static_cast<uint64_t>(value*periodNs);
    if (highTime > periodNs) highTime = periodNs;
    dutyCycleFile.seekp(0);
    dutyCycleFile << highTime << '\n';
}

void AnalogOut::freeSpin(int ch) {
    if(motorPins.has_value()) {
        auto& dirA = motorPins->dirA;
        auto& dirB = motorPins->dirB;
        auto offA = motorPins->offsetA;
        auto offB = motorPins->offsetB;
        dirB.set_value(offB, gpiod::line::value::INACTIVE);
        dirA.set_value(offA, gpiod::line::value::INACTIVE);
    }
}

void AnalogOut::brake(int ch) {
    if(motorPins.has_value()) {
        auto& dirA = motorPins->dirA;
        auto& dirB = motorPins->dirB;
        auto offA = motorPins->offsetA;
        auto offB = motorPins->offsetB;
        dirB.set_value(offB, gpiod::line::value::ACTIVE);
        dirA.set_value(offA, gpiod::line::value::ACTIVE);
    }
}

extern "C"{
  eeros::hal::ScalableOutput<double> *createAnalogOut(std::string id, void *libHandle, std::string device, uint32_t subDeviceNumber,
                                                      uint32_t channel, double scale, double offset, double rangeMin, double rangeMax, std::string unit) {
    return new bbblue::AnalogOut(id, libHandle, device, subDeviceNumber, channel, scale, offset, rangeMin, rangeMax, unit);
  }

  void freeSpin(bbblue::AnalogOut *obj, int ch) {
    obj->freeSpin(ch);
  }

  void brake(bbblue::AnalogOut *obj, int ch) {
    obj->brake(ch);
  }
}
