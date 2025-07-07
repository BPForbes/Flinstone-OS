
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void tcp_echo_server(int port) {
    int srv = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port = htons(port)
    };
    bind(srv, (struct sockaddr*)&addr, sizeof(addr));
    listen(srv, 5);
    printf("[TCP] Echo server listening on port %d\n", port);
    while (1) {
        int client = accept(srv, NULL, NULL);
        char buf[512];
        int n = read(client, buf, sizeof(buf));
        if (n > 0) {
            write(client, buf, n);
        }
        close(client);
    }
}
