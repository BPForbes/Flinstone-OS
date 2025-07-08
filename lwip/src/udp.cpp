#include "../include/udp.hpp"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>

UDPSocket::UDPSocket() : sockfd(-1) {}
UDPSocket::~UDPSocket() { close(); }

bool UDPSocket::bind(uint16_t port) {
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) return false;
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    return ::bind(sockfd, (sockaddr*)&addr, sizeof(addr)) == 0;
}

ssize_t UDPSocket::send_to(const std::string& host, uint16_t port, const void* buf, size_t len) {
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (::inet_pton(AF_INET, host.c_str(), &addr.sin_addr) <= 0) return -1;
    return ::sendto(sockfd, buf, len, 0, (sockaddr*)&addr, sizeof(addr));
}

ssize_t UDPSocket::recv_from(void* buf, size_t len, std::string& src_ip, uint16_t& src_port) {
    sockaddr_in addr{};
    socklen_t addrlen = sizeof(addr);
    ssize_t ret = ::recvfrom(sockfd, buf, len, 0, (sockaddr*)&addr, &addrlen);
    if (ret >= 0) {
        char tmp[INET_ADDRSTRLEN];
        ::inet_ntop(AF_INET, &addr.sin_addr, tmp, sizeof(tmp));
        src_ip = tmp;
        src_port = ntohs(addr.sin_port);
    }
    return ret;
}

void UDPSocket::close() {
    if (sockfd >= 0) {
        ::close(sockfd);
        sockfd = -1;
    }
}
