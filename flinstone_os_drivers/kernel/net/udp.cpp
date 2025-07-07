#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include "udp.hpp"

void udp_handle(uint8_t* data, size_t len) {
    struct udphdr* udp = (struct udphdr*)data;
    printf("UDP Packet Received: Src Port=%d, Dst Port=%d\n",
           ntohs(udp->source), ntohs(udp->dest));
}