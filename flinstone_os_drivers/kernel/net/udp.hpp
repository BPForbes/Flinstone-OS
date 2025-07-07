#ifndef UDP_H
#define UDP_H

#include <stdint.h>
#include <stddef.h>

struct udphdr {
    uint16_t source;
    uint16_t dest;
    uint16_t len;
    uint16_t check;
} __attribute__((packed));

void udp_handle(uint8_t* data, size_t len);

#endif
