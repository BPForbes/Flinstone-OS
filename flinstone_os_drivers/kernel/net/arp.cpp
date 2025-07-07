#include "arp.h"
#include <string.h>
#include <stdio.h>

#define ETH_TYPE_ARP 0x0806
#define ETH_TYPE_IP  0x0800

extern void nic_send_frame(uint8_t *frame, int len);
extern uint8_t local_mac[6];
extern uint32_t local_ip;

void etharp_query(uint32_t target_ip) {
    uint8_t pkt[42] = {0};
    struct arp_hdr *arp = (struct arp_hdr *)(pkt + 14);
    memset(pkt, 0xFF, 6); // Broadcast
    memcpy(pkt + 6, local_mac, 6);
    pkt[12] = 0x08; pkt[13] = 0x06; // ARP

    arp->htype = 0x0100;
    arp->ptype = 0x0008;
    arp->hlen = 6;
    arp->plen = 4;
    arp->oper = 0x0100; // ARP request
    memcpy(arp->sha, local_mac, 6);
    arp->spa = local_ip;
    memset(arp->tha, 0x00, 6);
    arp->tpa = target_ip;

    nic_send_frame(pkt, 42);
}

void etharp_input(uint8_t *frame, int len) {
    struct arp_hdr *arp = (struct arp_hdr *)(frame + 14);
    if (arp->oper == 0x0200) {
        printf("Received ARP reply from IP: %x
", arp->spa);
    }
}