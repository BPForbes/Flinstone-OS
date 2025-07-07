#include "wifi.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <linux/wireless.h>
#include <net/if.h>
#include <fcntl.h>

int wifi_scan(char* buf, int buflen) {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    struct iwreq req;
    strncpy(req.ifr_name, "wlan0", IFNAMSIZ);
    req.u.data.pointer = buf;
    req.u.data.length = buflen;
    if(ioctl(sock, SIOCGIWSCAN, &req)<0){ close(sock); return -1;}
    close(sock);
    return 0;
}

int wifi_connect(const char* ssid, const char* pass) {
    if (!ssid || !pass) return -1;

    pid_t pid = fork();
    if (pid < 0) {
        return -1;
    } else if (pid == 0) {
        char *const argv[] = {
            (char*)"iwconfig",
            (char*)"wlan0",
            (char*)"essid",
            (char*)ssid,
            (char*)"key",
            (char*)pass,
            NULL
        };
        execvp("iwconfig", argv);
        _exit(127);
    }

    int status = 0;
    if (waitpid(pid, &status, 0) < 0)
        return -1;
    if (WIFEXITED(status))
        return WEXITSTATUS(status);
    return -1;
}
