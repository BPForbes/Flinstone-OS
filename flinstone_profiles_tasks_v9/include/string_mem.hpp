#ifndef STRING_MEM_H
#define STRING_MEM_H
#include <stddef.h>
size_t strlen(const char *s);
char   *strcpy(char *dst, const char *src);
int     strcmp(const char *a, const char *b);
void   *memcpy(void *dst, const void *src, size_t n);
void   *memset(void *dst, int c, size_t n);
int     memcmp(const void *a, const void *b, size_t n);
#endif /* STRING_MEM_H */
