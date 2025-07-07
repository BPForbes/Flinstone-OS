#include "socket.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <stdio.h>

int sock_open_udp(uint16_t port) {
    int s = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in addr = {AF_INET, htons(port), {0}};
    if (bind(s, (struct sockaddr*)&addr, sizeof(addr))<0) { close(s); return -1; }
    return s;
}

int sock_sendto(int sock, const void* data, int len, const char* dst_ip, uint16_t dst_port) {
    struct sockaddr_in addr = {AF_INET, htons(dst_port)};
    inet_pton(AF_INET, dst_ip, &addr.sin_addr);
    return sendto(sock, data, len, 0, (struct sockaddr*)&addr, sizeof(addr));
}

int sock_recvfrom(int sock, void* buf, int maxlen, char* src_ip, uint16_t* src_port) {
    struct sockaddr_in addr;
    socklen_t alen = sizeof(addr);
    int r = recvfrom(sock, buf, maxlen, 0, (struct sockaddr*)&addr, &alen);
    inet_ntop(AF_INET, &addr.sin_addr, src_ip, 16);
    *src_port = ntohs(addr.sin_port);
    return r;
}