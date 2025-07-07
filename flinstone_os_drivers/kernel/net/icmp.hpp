#ifndef ICMP_H
#define ICMP_H

#include <stdint.h>

#define ICMP_ECHO       8
#define ICMP_ECHOREPLY  0

struct icmp_hdr {
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    uint16_t id;
    uint16_t seq;
} __attribute__((packed));

uint16_t icmp_checksum(void* buf, int len);
int icmp_send_echo(uint32_t dest_ip, uint16_t id, uint16_t seq, const uint8_t* data, int data_len);
int icmp_receive(struct icmp_hdr* hdr, uint8_t* data, int maxlen);
#endif