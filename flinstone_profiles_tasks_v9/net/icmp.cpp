
#include <netinet/ip_icmp.h>
#include <netinet/ip.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>

void icmp_send_echo(const char* ip_str) {
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, ip_str, &addr.sin_addr);

    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sock < 0) {
        perror("socket"); return;
    }

    struct icmp icmp_hdr;
    memset(&icmp_hdr, 0, sizeof(icmp_hdr));
    icmp_hdr.icmp_type = ICMP_ECHO;
    icmp_hdr.icmp_code = 0;
    icmp_hdr.icmp_id = getpid() & 0xFFFF;
    icmp_hdr.icmp_seq = 0;
    icmp_hdr.icmp_cksum = 0;

    char packet[64];
    memcpy(packet, &icmp_hdr, sizeof(icmp_hdr));
    strcpy(packet + sizeof(icmp_hdr), "ping");
    int len = sizeof(icmp_hdr) + strlen("ping");
    icmp_hdr.icmp_cksum = 0;

    unsigned short *buf = (unsigned short *)packet;
    int nleft = len;
    unsigned int sum = 0;
    while (nleft > 1) {
        sum += *buf++;
        nleft -= 2;
    }
    if (nleft == 1)
        sum += *(unsigned char *)buf;
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    icmp_hdr.icmp_cksum = ~sum;

    memcpy(packet, &icmp_hdr, sizeof(icmp_hdr));
    sendto(sock, packet, len, 0, (struct sockaddr*)&addr, sizeof(addr));
    printf("[ICMP] Echo sent to %s\n", ip_str);
    close(sock);
}
