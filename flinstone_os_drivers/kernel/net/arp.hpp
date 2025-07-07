#ifndef ARP_H
#define ARP_H

#include <stdint.h>

typedef struct {
    uint16_t htype;
    uint16_t ptype;
    uint8_t hlen;
    uint8_t plen;
    uint16_t oper;
    uint8_t sha[6];
    uint32_t spa;
    uint8_t tha[6];
    uint32_t tpa;
} __attribute__((packed)) arp_hdr;

void etharp_input(uint8_t* frame, int len);
void etharp_query(uint32_t target_ip);

#endif
