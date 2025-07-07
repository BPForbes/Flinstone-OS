#include <stdio.h>
#include <stdint.h>
#include <string.h>

int logging_enabled = 0;

void pktlog_toggle() {
    logging_enabled = !logging_enabled;
    printf("Packet log %s\n", logging_enabled ? "ENABLED" : "DISABLED");
}

void pktlog_dump(const uint8_t *buf, int len) {
    if (!logging_enabled) return;
    printf("Packet [%d bytes]:\n", len);
    for (int i = 0; i < len && i < 64; ++i) {
        printf("%02X ", buf[i]);
        if ((i+1)%16 == 0) printf("\n");
    }
    printf("\n");
}