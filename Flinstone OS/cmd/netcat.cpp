#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <cstring>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: netcat <ip> <port>\n";
        return 1;
    }
    const char* ip = argv[1];
    int port = std::stoi(argv[2]);
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) { perror("socket"); return 1; }
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &addr.sin_addr);
    if (connect(sock, (sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("connect"); return 1;
    }
    fd_set fds;
    while (true) {
        FD_ZERO(&fds);
        FD_SET(0, &fds);
        FD_SET(sock, &fds);
        if (select(sock+1, &fds, nullptr, nullptr, nullptr) < 0) break;
        if (FD_ISSET(0, &fds)) {
            char buf[512];
            int n = read(0, buf, sizeof(buf));
            if (n <= 0) break;
            write(sock, buf, n);
        }
        if (FD_ISSET(sock, &fds)) {
            char buf[512];
            int n = read(sock, buf, sizeof(buf));
            if (n <= 0) break;
            write(1, buf, n);
        }
    }
    close(sock);
    return 0;
}