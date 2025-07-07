#include "wifi.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
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
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "iwconfig wlan0 essid %s key %s", ssid, pass);
    return system(cmd);
}