#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "ip.h"

struct icmp_hdr {
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    uint16_t id;
    uint16_t seq;
    uint8_t data[32];
} __attribute__((packed));

static uint16_t icmp_checksum(void *buf, int len) {
    uint16_t *data = (uint16_t *)buf;
    uint32_t sum = 0;
    for (; len > 1; len -= 2)
        sum += *data++;
    if (len == 1)
        sum += *(uint8_t *)data;
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    return ~sum;
}

void icmp_send_echo(uint32_t dst_ip, uint16_t id, uint16_t seq) {
    struct icmp_hdr pkt;
    pkt.type = 8;  // Echo request
    pkt.code = 0;
    pkt.id = id;
    pkt.seq = seq;
    memset(pkt.data, 0xAB, sizeof(pkt.data));
    pkt.checksum = 0;
    pkt.checksum = icmp_checksum(&pkt, sizeof(pkt));
    ip_output((uint8_t *)&pkt, sizeof(pkt), dst_ip, 1);
}

void icmp_input(uint8_t *packet, int len) {
    struct icmp_hdr *hdr = (struct icmp_hdr *)packet;
    if (hdr->type == 0) {
        printf("Reply: seq=%u
", hdr->seq);
    }
}