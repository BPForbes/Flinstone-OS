#ifndef STORAGE_H
#define STORAGE_H
#include <stddef.h>
int read_block(size_t block_index, void *buffer);
int write_block(size_t block_index, const void *buffer);
#endif /* STORAGE_H */
