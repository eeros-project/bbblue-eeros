#pragma once

//#define DEBUG

#ifdef DEBUG
#include <iostream>
#include <utility>
#endif

struct Logger {
    template<typename T>
    Logger& operator<<(T&& t) {
        #ifdef DEBUG
        std::cout << std::forward<T>(t);
        #endif
        return *this;
    }
};
Logger& debug();
