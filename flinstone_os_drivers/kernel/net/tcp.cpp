#include <stdio.h>
#include "tcp.hpp"

void tcp_handle(uint8_t* data, size_t len) {
    printf("TCP Packet Received: %zu bytes\n", len);
}