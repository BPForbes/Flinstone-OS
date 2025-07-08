#ifndef LWIP_ETHARP_HPP
#define LWIP_ETHARP_HPP

#include <cstdint>

void etharp_query(uint32_t target_ip);

#endif
