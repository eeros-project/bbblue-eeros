#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cerrno>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <fcntl.h>
#include <ios>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <string_view>
#include <sys/types.h>
#include <unistd.h>
#include <utility>

struct SysfsFile {
    int fd = -1;
    std::string path;

    explicit SysfsFile(std::string p) {
        fd = open(p.c_str(), O_RDWR);
        if(fd < 0) throw std::runtime_error(p + " could not be opened: " + strerror(errno));
        path = std::move(p);
    }

    ~SysfsFile() {
        if(fd > 0) close(fd);
    }
    SysfsFile() {}
    SysfsFile(const SysfsFile&) = delete;
    SysfsFile& operator=(const SysfsFile&) = delete;
    SysfsFile(SysfsFile&& other) {
        std::swap(path, other.path);
        std::swap(fd, other.fd);
    }
    SysfsFile& operator=(SysfsFile&& other) {
        std::swap(path, other.path);
        std::swap(fd, other.fd);
        return *this;
    }

    template<std::size_t N>
    std::size_t readIntoBuffer(std::array<uint8_t, N>& buffer) {
        auto err = lseek(fd, 0, SEEK_SET);
        if(err == -1) {
            throw std::runtime_error(path + ": seek failed: " + strerror(errno));
        }
        ssize_t n = read(fd, buffer.data(), buffer.size());
        if (n < 0) {
            throw std::runtime_error(path + "read failed: " + strerror(errno));
        }
        return std::size_t(n);
    }

    template<typename T, std::size_t N>
    void parseDigits(T& value, const std::array<uint8_t, N>& buffer, std::size_t n, std::size_t start = 0) {
        uint32_t multiplier = 1;
        for(std::size_t i = n-1; i >= start && i < n; --i) {
            if(i > 100) throw std::runtime_error("wtf");
            auto digit = buffer[i];
            if(digit >= '0' && digit <= '9') {
                value += (buffer[i] - '0')*multiplier;
                multiplier*=10;
            } else {
                switch(digit) {
                    case '\0':
                    case '\n':
                        break;
                    default:
                        std::cout << path << ": invalid digit at " << std::dec << i << ": '" << digit << "'(0x" << std::hex << (int)digit << ")\n";
                }
            }
        }
    }

    uint32_t readU32() {
        std::array<uint8_t, 10> bytes;
        auto n = readIntoBuffer(bytes);
        assert(n > 0);
        uint32_t value = 0;
        parseDigits(value, bytes, n);
        // std::cout << path << " = " << std::dec << value << '\n';
        return value;
    }

    int32_t readI32() {
        std::array<uint8_t, 11> bytes;
        int32_t value = 0;
        auto n = readIntoBuffer(bytes);
        if(bytes[0] == '-') {
            parseDigits(value, bytes, n, 1);
            value *= -1;
        } else {
            parseDigits(value, bytes, n);
        }
        return value;
    }

    double readDecimal() {
        std::array<uint8_t, 24> bytes;
        auto n = readIntoBuffer(bytes);
        auto offset = bytes[0] == '-' ? 1 : 0;
        auto start = bytes.cbegin() + offset;
        auto end = bytes.cbegin() + n - 1;
        auto dot = std::find(start, end, '.');
        auto exponent = std::distance(start, dot)-1;
        double multiplier = std::pow(10.0, exponent);
        double value = 0;
        for(auto i = offset; i < n; ++i) {
            auto digit = bytes[i];
            if(digit == '.' || digit == '\n') continue;
            if(digit < '0' || digit > '9') {
                std::cout << "invalid digit at " << std::dec << (int)i << ": " << digit << " (0x" << std::hex << (int)digit << ")\n";
                continue;
            }
            value += (digit-'0')*multiplier;
            multiplier /= 10;
        }
        if(bytes[0] == '-') value *= -1;
        return value;
    }

    void writeU32(uint32_t value) {
        auto err = lseek(fd, 0, SEEK_SET);
        if(err == -1) {
            throw std::runtime_error(path + ": seek failed: " + strerror(errno));
        }
        auto text = std::to_string(value);
        text.append("\n");
        auto n = write(fd, text.c_str(), text.size());
        if(n < 0) throw std::runtime_error(path + ": write failed: " + strerror(errno) + strerror(errno));
    }

};
