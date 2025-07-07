#ifndef WIFI_H
#define WIFI_H

int wifi_scan(char* buf, int buflen);
int wifi_connect(const char* ssid, const char* pass);
#endif