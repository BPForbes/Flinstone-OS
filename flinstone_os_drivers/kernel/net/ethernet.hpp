#ifndef ETHERNET_H
#define ETHERNET_H

#include <stdint.h>
#include <stddef.h>

extern uint8_t local_mac[6];

typedef struct {
    uint8_t dest_mac[6];
    uint8_t src_mac[6];
    uint16_t ethertype;
    uint8_t payload[1500];
} __attribute__((packed)) ethernet_frame_t;

void ethernet_handle(uint8_t* packet, size_t len);
void ethernet_send(uint8_t* dst_mac, uint16_t ethertype, void* payload, size_t len);

#endif
