#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <unistd.h>
#include <chrono>

uint16_t checksum(void* buf, int len) {
    uint16_t* data = (uint16_t*)buf;
    uint32_t sum = 0;
    for (; len > 1; len -= 2) sum += *data++;
    if (len) sum += *(uint8_t*)data;
    while (sum >> 16) sum = (sum & 0xFFFF) + (sum >> 16);
    return ~sum;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ping <ip>\n";
        return 1;
    }
    const char* ip = argv[1];
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sock < 0) { perror("socket"); return 1; }
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &addr.sin_addr);
    icmp icmp_req{};
    memset(&icmp_req, 0, sizeof(icmp_req));
    icmp_req.icmp_type = ICMP_ECHO;
    icmp_req.icmp_hun.ih_idseq.icd_id = htons(1);
    icmp_req.icmp_hun.ih_idseq.icd_seq = htons(1);
    icmp_req.icmp_cksum = checksum(&icmp_req, sizeof(icmp_req));
    auto start = std::chrono::steady_clock::now();
    if (sendto(sock, &icmp_req, sizeof(icmp_req), 0, (sockaddr*)&addr, sizeof(addr)) <= 0) {
        perror("sendto"); return 1;
    }
    uint8_t buf[1024];
    socklen_t len = sizeof(addr);
    if (recvfrom(sock, buf, sizeof(buf), 0, (sockaddr*)&addr, &len) <= 0) {
        perror("recvfrom"); return 1;
    }
    auto end = std::chrono::steady_clock::now();
    std::cout << "Reply from " << ip << ": time=" 
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " ms\n";
    close(sock);
    return 0;
}