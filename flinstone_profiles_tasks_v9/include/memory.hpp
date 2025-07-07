#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>
#include <sys/types.h>

/* Bump allocator and free-list malloc/free from ASM */
void *sbrk(ptrdiff_t increment);
void *malloc(size_t size);
void  free(void *ptr);

#endif /* MEMORY_H */