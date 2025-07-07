#ifndef DNS_H
#define DNS_H

#include <stdint.h>

int dns_query(const char* server_ip_str, const char* hostname, char* out_ip);
#endif