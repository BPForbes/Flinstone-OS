#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <net/ethernet.h>
#include <netinet/if_ether.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <string.h>
#include <stdio.h>

void udp_handle(uint8_t* data, size_t len) {
    udp_header_t* udp = (udp_header_t*)data;
    printf("UDP Packet Received: Src Port=%d, Dst Port=%d\n",
           ntohs(udp->src_port), ntohs(udp->dst_port));
}