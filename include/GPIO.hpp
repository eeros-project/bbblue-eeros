#pragma once

#include <gpiod.hpp>
#include <fstream>

struct LEDInfo {
    uint32_t channel;
    std::filesystem::path path;
};

struct LED {
    std::fstream file;
    bool inverted;
    std::filesystem::path triggerPath;
    std::string originalTrigger;
};

struct GPIOInfo {
    uint32_t channel;
    std::filesystem::path path;
    gpiod::line::offset offset;
};

struct GPIO {
    gpiod::chip chip;
    gpiod::line_request request;
    gpiod::line::offset offset;
};
