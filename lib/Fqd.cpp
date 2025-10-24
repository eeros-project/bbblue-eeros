#include "../include/Fqd.hpp"
#include <array>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <dirent.h>
#include <iostream>
#include <string>
#include <Log.hpp>

using namespace bbblue;
using namespace eeros::hal;

static const std::array<const char*,4> counterNames = {
    "48300180.counter",
    "48302180.counter",
    "48304180.counter",
    "librc_pru"
};

Fqd::Fqd(std::string id,  void* libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel, double scale,
    double offset, double rangeMin, double rangeMax, std::string unit, bool getDelta)
    : ScalableInput<double>(id, libHandle, scale, offset, rangeMin, rangeMax, unit), channel(channel), prevPos(0) {
        std::string countPath;
        std::string counterBasePath{"/sys/bus/counter/devices/"};
        auto counterDeviceDirectory = opendir(counterBasePath.c_str());
        if(counterDeviceDirectory == nullptr) {
            std::cout << "counterDeviceDirectory is NULL!\n";
            std::exit(1);
        }
        while(true) {
            auto dir = readdir(counterDeviceDirectory);
            if(dir == nullptr) break;
            std::string_view dirname{dir->d_name};
            if(dirname == "." || dirname == "..") continue;
            debug() << "found Counter " << dirname << '\n';
            std::string counterPath = counterBasePath + dirname.data();
            std::string counterNamePath = counterPath + "/name";
            std::string name;
            std::ifstream{counterNamePath} >> name;
            if(name == counterNames[channel-1]) {
                countPath = counterPath + "/count0";
                break;
            }
        }
        closedir(counterDeviceDirectory);
        debug() << "identified " << countPath << " as " << counterNames[channel-1] << '\n';
        enablePath = countPath + "/enable";
        std::ofstream{enablePath} << "1\n";
        std::string count0countPath = countPath + "/count";
        countFile = SysfsFile(count0countPath);
        reset();
        pos=offset/scale;
}

Fqd::~Fqd() {
  std::ofstream{enablePath} << "0\n";
}


double Fqd::get() {
  uint32_t actPos;
  actPos = countFile.readU32();
  int32_t signedActPos;
  std::memcpy(&signedActPos, &actPos, sizeof(actPos));
  int32_t deltaPos = signedActPos - prevPos;
  double increment = deltaPos / scale;
  pos += increment;
  prevPos = signedActPos;
  return pos;
}

void Fqd::reset() {
  debug() << "resetting FQD " << channel << '\n';
  countFile.writeU32(0);
}

void Fqd::setPos(double position){
  pos = position;
  prevPos = pos*scale+offset;
  uint32_t unsignedPos;
  std::memcpy(&unsignedPos, &prevPos, sizeof(unsignedPos));
  debug() << "setting FQD " << channel << " to " << std::dec << pos << '/' << scale << " = " << prevPos << " = " << unsignedPos << '\n';
  countFile.writeU32(unsignedPos);
}

extern "C"{
  eeros::hal::ScalableInput<double> *createFqd(std::string id, void* libHandle, std::string device, uint32_t subDeviceNumber, uint32_t channel,
      double scale, double offset, double rangeMin, double rangeMax, std::string unit) {
    return new bbblue::Fqd(id, libHandle, device, subDeviceNumber, channel, scale, offset, rangeMin, rangeMax, unit);
  }

  void resetFqd(bbblue::Fqd *obj){
    obj->reset();
  }

  void setFqdPos(bbblue::Fqd *obj, double position){
    obj->setPos(position);
  }
}
