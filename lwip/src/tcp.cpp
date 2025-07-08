#include "../include/tcp.hpp"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>

TCPServer::TCPServer() : sockfd(-1) {}
TCPServer::~TCPServer() { close(); }

bool TCPServer::listen(uint16_t port) {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) return false;
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    if (bind(sockfd, (sockaddr*)&addr, sizeof(addr)) < 0) return false;
    return ::listen(sockfd, 5) == 0;
}

int TCPServer::accept() {
    return ::accept(sockfd, nullptr, nullptr);
}

ssize_t TCPServer::send(int client_fd, const void* buf, size_t len) {
    return ::send(client_fd, buf, len, 0);
}

ssize_t TCPServer::receive(int client_fd, void* buf, size_t len) {
    return ::recv(client_fd, buf, len, 0);
}

void TCPServer::close() {
    if (sockfd >= 0) {
        ::close(sockfd);
        sockfd = -1;
    }
}

TCPClient::TCPClient() : sockfd(-1) {}
TCPClient::~TCPClient() { close(); }

bool TCPClient::connect(const std::string& host, uint16_t port) {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) return false;
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (::inet_pton(AF_INET, host.c_str(), &addr.sin_addr) <= 0) return false;
    return ::connect(sockfd, (sockaddr*)&addr, sizeof(addr)) == 0;
}

ssize_t TCPClient::send(const void* buf, size_t len) {
    return ::send(sockfd, buf, len, 0);
}

ssize_t TCPClient::receive(void* buf, size_t len) {
    return ::recv(sockfd, buf, len, 0);
}

void TCPClient::close() {
    if (sockfd >= 0) {
        ::close(sockfd);
        sockfd = -1;
    }
}
