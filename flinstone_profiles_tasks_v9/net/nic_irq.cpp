
#include <stdio.h>
#include <stdint.h>

void nic_handle_irq() {
    printf("[IRQ] NIC interrupt triggered, waking RX loop\n");
    // In real system, this would notify RX thread or poll LWIP
}
