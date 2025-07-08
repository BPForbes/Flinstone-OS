#ifndef LWIP_TCP_HPP
#define LWIP_TCP_HPP

#include <string>
#include <cstddef>
#include <cstdint>

class TCPServer {
public:
    TCPServer();
    ~TCPServer();
    bool listen(uint16_t port);
    int accept();
    ssize_t send(int client_fd, const void* buf, size_t len);
    ssize_t receive(int client_fd, void* buf, size_t len);
    void close();
private:
    int sockfd;
};

class TCPClient {
public:
    TCPClient();
    ~TCPClient();
    bool connect(const std::string& host, uint16_t port);
    ssize_t send(const void* buf, size_t len);
    ssize_t receive(void* buf, size_t len);
    void close();
private:
    int sockfd;
};

#endif
