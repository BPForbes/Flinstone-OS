#ifndef LWIP_DHCP_HPP
#define LWIP_DHCP_HPP

#include "ip4.hpp"

class DHCPClient {
public:
    IPv4Address request_address();
};

#endif
