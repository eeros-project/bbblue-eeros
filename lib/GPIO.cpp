#include <GPIO.hpp>
#include <gpiod.hpp>
#include <iostream>
#include <stdexcept>
#include <string_view>
#include <filesystem>

GPIO requestGPIO(std::string name, gpiod::line::direction direction, bool inverted) {
    auto devDir = std::filesystem::path{"/dev"};
    for (const auto& dev : std::filesystem::directory_iterator{devDir}) {
        if(!dev.is_character_file()) continue;
        if(!std::string_view{dev.path().filename().c_str()}.starts_with("gpiochip")) continue;
        auto chip = gpiod::chip{dev.path()};
        auto lineNumber = chip.get_line_offset_from_name(name);
        if (lineNumber == -1) continue;
        auto settings = gpiod::line_settings{}.set_direction(direction).set_active_low(inverted);
        auto request = chip.prepare_request().set_consumer("BBBlue EEROS").add_line_settings(lineNumber, settings).do_request();
        std::cout << "identified " << name << " as " << dev.path() << " line " << lineNumber << '\n';
        return std::move(GPIO{std::move(chip), std::move(request), lineNumber});
    }
    throw std::runtime_error("GPIO not found");
}
