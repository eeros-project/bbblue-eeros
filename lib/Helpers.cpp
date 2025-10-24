#include <Helpers.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <Log.hpp>
namespace bbblue {
    PHandle readPHandle(std::string path) {
        std::ifstream pHandleFile{path, std::ios::binary};
        PHandle handle = 0;
        pHandleFile.read(reinterpret_cast<char*>(&handle), sizeof(handle));
        debug() << "found phandle " << std::hex << handle << '\n';
        return handle;
    }

    PHandle deviceTreeSymbolToPHandle(std::string_view symbol) {
        std::string symbolFilePath{"/proc/device-tree/__symbols__/"};
        symbolFilePath.append(symbol);
        std::ifstream symbolFile{symbolFilePath};
        std::string actualPath;
        symbolFile >> actualPath;
        actualPath.pop_back();
        debug() << "found actual path for symbol " << symbol << ": " << actualPath << '\n';
        std::string pHandlePath{"/proc/device-tree"};
        pHandlePath.append(actualPath);
        pHandlePath.append("/phandle");

        debug() << "phandle path: " << pHandlePath << '\n';
        return readPHandle(pHandlePath);
    }
}
