
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

void resolve_dns(const char* hostname) {
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(53);
    inet_pton(AF_INET, "8.8.8.8", &addr.sin_addr);

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) { perror("socket"); return; }

    unsigned char buf[512];
    memset(buf, 0, sizeof(buf));
    buf[0] = 0x12; buf[1] = 0x34; // Transaction ID
    buf[2] = 1;     // standard query
    buf[5] = 1;     // one question

    int len = 12;
    const char *h = hostname;
    while (*h) {
        const char *dot = strchr(h, '.');
        int labellen = dot ? dot - h : strlen(h);
        buf[len++] = labellen;
        memcpy(buf + len, h, labellen);
        len += labellen;
        if (!dot) break;
        h = dot + 1;
    }
    buf[len++] = 0; // end of hostname
    buf[len++] = 0; buf[len++] = 1; // Type A
    buf[len++] = 0; buf[len++] = 1; // Class IN

    sendto(sock, buf, len, 0, (struct sockaddr*)&addr, sizeof(addr));
    printf("[DNS] Query for %s sent\n", hostname);
    close(sock);
}
