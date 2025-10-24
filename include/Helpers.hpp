#pragma once

#include <cstdint>
#include <string_view>

namespace bbblue {

    using PHandle = uint32_t;

    PHandle readPHandle(std::string path);
    PHandle deviceTreeSymbolToPHandle(std::string_view symbol);

}
