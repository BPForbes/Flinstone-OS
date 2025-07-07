#include "packet_inspect.h"
#include <stdio.h>
#include "ethernet.h"
#include "ip.h"

void inspect_packet(const uint8_t* packet, int len) {
    printf("Packet len: %d\n", len);
    // Ethernet
    // assuming first 14 bytes
    // IP inside
    struct ip_hdr* iph = (void*)(packet+14);
    printf("IP src: %u.%u.%u.%u -> dst: %u.%u.%u.%u proto: %u\n",
        (iph->saddr)&0xFF,(iph->saddr>>8)&0xFF,(iph->saddr>>16)&0xFF,(iph->saddr>>24)&0xFF,
        (iph->daddr)&0xFF,(iph->daddr>>8)&0xFF,(iph->daddr>>16)&0xFF,(iph->daddr>>24)&0xFF,
        iph->protocol);
}