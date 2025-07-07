#include "firewall.h"
#include <stdlib.h>
#include <string.h>
#include "ip.h"

typedef struct {
    uint8_t proto;
    uint32_t src, dst;
    uint16_t port;
    rule_action act;
} fw_rule;

static fw_rule* rules = NULL;
static int rule_count = 0;

void fw_add_rule(uint8_t proto, uint32_t src, uint32_t dst, uint16_t port, rule_action act) {
    rules = realloc(rules, sizeof(fw_rule)*(rule_count+1));
    rules[rule_count++] = (fw_rule){proto, src, dst, port, act};
}

rule_action fw_checkPacket(const uint8_t* packet, int len) {
    struct ip_hdr* iph = (void*)(packet+14);
    for(int i=0;i<rule_count;i++){
        fw_rule*r=&rules[i];
        if(r->proto==iph->protocol && (r->src==0||r->src==iph->saddr)
          && (r->dst==0||r->dst==iph->daddr))
            return r->act;
    }
    return ALLOW;
}