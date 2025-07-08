#include "../include/etharp.hpp"
#include <iostream>

void etharp_query(uint32_t target_ip) {
    std::cout << "ARP query for IP: "
              << ((target_ip >> 24) & 0xFF) << '.'
              << ((target_ip >> 16) & 0xFF) << '.'
              << ((target_ip >> 8) & 0xFF) << '.'
              << (target_ip & 0xFF) << std::endl;
}
