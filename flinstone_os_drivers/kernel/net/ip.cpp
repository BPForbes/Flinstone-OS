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

void ip_handle(uint8_t* packet, size_t len) {
    ip_header_t* hdr = (ip_header_t*)packet;
    uint8_t protocol = hdr->protocol;
    void* payload = packet + sizeof(ip_header_t);
    size_t payload_len = len - sizeof(ip_header_t);

    if (protocol == 0x01) {
        icmp_handle(payload, payload_len);
    } else if (protocol == 0x11) {
        udp_handle(payload, payload_len);
    } else if (protocol == 0x06) {
        tcp_handle(payload, payload_len);
    }
}