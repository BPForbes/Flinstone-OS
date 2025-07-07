

#include "net/ethernet.h"
#include <stdio.h>

// Simulated RX hook from NIC driver
void nic_rx_handler(uint8_t* packet, size_t len) {
    printf("[NIC RX] Packet received, dispatching to Ethernet layer\n");
    ethernet_handle(packet, len);
}
