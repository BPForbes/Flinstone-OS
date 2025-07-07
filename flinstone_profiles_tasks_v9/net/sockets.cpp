
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>

int sock_open(int domain, int type, int protocol) {
    return socket(domain, type, protocol);
}

int sock_send(int fd, const void *buf, size_t len) {
    return send(fd, buf, len, 0);
}

int sock_recv(int fd, void *buf, size_t len) {
    return recv(fd, buf, len, 0);
}

void sock_close(int fd) {
    close(fd);
}
