#ifndef FIREWALL_H
#define FIREWALL_H

#include <stdint.h>

typedef enum { ALLOW, DROP } rule_action;
void fw_add_rule(uint8_t proto, uint32_t src, uint32_t dst, uint16_t port, rule_action act);
rule_action fw_checkPacket(const uint8_t* packet, int len);
#endif