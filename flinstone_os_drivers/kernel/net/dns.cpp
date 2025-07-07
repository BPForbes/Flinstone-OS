#include "dns.h"
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdint.h>

#pragma pack(push,1)
struct dns_hdr {
    uint16_t id, flags, qdcount, ancount, nscount, arcount;
};
#pragma pack(pop)

static void build_query(uint8_t* buf, const char* name, int* len) {
    struct dns_hdr* hdr = (void*)buf;
    hdr->id = htons(0x1234);
    hdr->flags = htons(0x0100);
    hdr->qdcount = htons(1);
    hdr->ancount = hdr->nscount = hdr->arcount = 0;
    uint8_t* q = buf + sizeof(*hdr);
    char hostcpy[256]; strcpy(hostcpy, name);
    char* token = strtok(hostcpy, ".");
    while (token) {
        int l = strlen(token);
        *q++ = l;
        memcpy(q, token, l); q += l;
        token = strtok(NULL, ".");
    }
    *q++ = 0;
    *(uint16_t*)q = htons(1); q += 2; // QTYPE A
    *(uint16_t*)q = htons(1); q += 2; // QCLASS IN
    *len = q - buf;
}

int dns_query(const char* server_ip_str, const char* hostname, char* out_ip) {
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) return -1;
    struct sockaddr_in serv = {AF_INET};
    serv.sin_port = htons(53);
    inet_pton(AF_INET, server_ip_str, &serv.sin_addr);
    uint8_t buf[512]; int buflen;
    build_query(buf, hostname, &buflen);
    sendto(sock, buf, buflen, 0, (struct sockaddr*)&serv, sizeof(serv));
    int len = recv(sock, buf, sizeof(buf), 0);
    close(sock);
    if (len < (int)sizeof(struct dns_hdr)) return -1;
    struct dns_hdr* hdr = (void*)buf;
    int pos = sizeof(*hdr) + strlen(hostname) + 2 + 4;
    // Skip question
    pos = sizeof(*hdr);
    // Parse answer
    // Jump to answer start
    pos += strlen(hostname) + 2 + 4;
    // Read RDATA at end
    uint8_t* ptr = buf + len - 4;
    snprintf(out_ip, 16, "%u.%u.%u.%u", ptr[0],ptr[1],ptr[2],ptr[3]);
    return 0;
}