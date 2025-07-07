
#include "net/dhcp.h"
#include <stdio.h>

void dhcp_start(void* iface) {
    printf("Sending DHCPDISCOVER on interface %p\n", iface);
}
