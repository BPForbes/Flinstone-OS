#include "../include/ip4.hpp"
#include <sstream>
#include <vector>

IPv4Address IPv4Address::from_string(const std::string& str) {
    IPv4Address addr;
    std::stringstream ss(str);
    std::string item;
    int i = 0;
    while (std::getline(ss, item, '.') && i < 4) {
        addr.octets[i++] = static_cast<uint8_t>(std::stoi(item));
    }
    return addr;
}

std::string IPv4Address::to_string() const {
    std::ostringstream oss;
    oss << static_cast<int>(octets[0]) << '.'
        << static_cast<int>(octets[1]) << '.'
        << static_cast<int>(octets[2]) << '.'
        << static_cast<int>(octets[3]);
    return oss.str();
}
