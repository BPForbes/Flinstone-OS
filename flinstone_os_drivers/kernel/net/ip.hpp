#ifndef IP_H
#define IP_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint8_t ihl_version;
    uint8_t tos;
    uint16_t tot_len;
    uint16_t id;
    uint16_t frag_off;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t checksum;
    uint32_t src_ip;
    uint32_t dst_ip;
} __attribute__((packed)) ip_header_t;

void ip_handle(uint8_t* packet, size_t len);

#endif
