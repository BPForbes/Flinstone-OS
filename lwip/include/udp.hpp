#ifndef LWIP_UDP_HPP
#define LWIP_UDP_HPP

#include <string>
#include <cstddef>
#include <cstdint>

class UDPSocket {
public:
    UDPSocket();
    ~UDPSocket();
    bool bind(uint16_t port);
    ssize_t send_to(const std::string& host, uint16_t port, const void* buf, size_t len);
    ssize_t recv_from(void* buf, size_t len, std::string& src_ip, uint16_t& src_port);
    void close();
private:
    int sockfd;
};

#endif
