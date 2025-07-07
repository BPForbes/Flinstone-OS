#ifndef PACKET_INSPECT_H
#define PACKET_INSPECT_H

#include <stdint.h>

void inspect_packet(const uint8_t* packet, int len);
#endif