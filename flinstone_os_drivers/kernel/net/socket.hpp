#ifndef SOCKET_H
#define SOCKET_H

#include <stdint.h>

int sock_open_udp(uint16_t port);
int sock_sendto(int sock, const void* data, int len, const char* dst_ip, uint16_t dst_port);
int sock_recvfrom(int sock, void* buf, int maxlen, char* src_ip, uint16_t* src_port);
#endif