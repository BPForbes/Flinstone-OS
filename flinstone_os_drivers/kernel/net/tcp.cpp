#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <net/ethernet.h>
#include <netinet/if_ether.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <stdio.h>

void tcp_handle(uint8_t* data, size_t len) {
    printf("TCP Packet Received: %zu bytes\n", len);
}