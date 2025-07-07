#include <string.h>
#include <arpa/inet.h>
#include "ethernet.hpp"
#include "ip.hpp"
#include "arp.hpp"

void ethernet_handle(uint8_t* packet, size_t len) {
    ethernet_frame_t* frame = (ethernet_frame_t*)packet;
    if (ntohs(frame->ethertype) == 0x0800) {
        ip_handle(frame->payload, len - sizeof(ethernet_frame_t));
    } else if (ntohs(frame->ethertype) == 0x0806) {
        arp_handle(frame->payload, len - sizeof(ethernet_frame_t));
    }
}

void ethernet_send(uint8_t* dst_mac, uint16_t ethertype, void* payload, size_t len) {
    ethernet_frame_t frame;
    memcpy(frame.dest_mac, dst_mac, 6);
    memcpy(frame.src_mac, local_mac, 6);
    frame.ethertype = htons(ethertype);
    memcpy(frame.payload, payload, len);
    nic_send((uint8_t*)&frame, len + sizeof(ethernet_frame_t));
}