#ifndef LWIP_IP4_HPP
#define LWIP_IP4_HPP

#include <string>
#include <cstdint>

struct IPv4Address {
    uint8_t octets[4]{};
    IPv4Address() = default;
    IPv4Address(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
        octets[0] = a; octets[1] = b; octets[2] = c; octets[3] = d;
    }
    static IPv4Address from_string(const std::string& str);
    std::string to_string() const;
};

#endif
