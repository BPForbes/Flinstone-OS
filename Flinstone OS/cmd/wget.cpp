#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: wget <url>\n";
        return 1;
    }
    std::string url = argv[1];
    // Basic parse: assume http://
    std::string host = url.substr(7);
    std::string path = "/";
    size_t pos = host.find('/');
    if (pos != std::string::npos) {
        path = host.substr(pos);
        host = host.substr(0, pos);
    }
    int port = 80;
    pos = host.find(':');
    if (pos != std::string::npos) {
        port = std::stoi(host.substr(pos+1));
        host = host.substr(0, pos);
    }
    addrinfo hints{}, *res;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(host.c_str(), std::to_string(port).c_str(), &hints, &res)) {
        std::cerr << "getaddrinfo failed\n"; return 1;
    }
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    connect(sock, res->ai_addr, res->ai_addrlen);
    std::string req = "GET " + path + " HTTP/1.0\r\nHost: " + host + "\r\n\r\n";
    send(sock, req.c_str(), req.size(), 0);
    char buf[1024];
    int n;
    while ((n = read(sock, buf, sizeof(buf))) > 0) {
        write(1, buf, n);
    }
    close(sock);
    return 0;
}