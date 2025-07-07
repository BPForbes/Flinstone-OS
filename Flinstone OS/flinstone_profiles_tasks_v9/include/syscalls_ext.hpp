#ifndef SYSCALLS_EXT_H
#define SYSCALLS_EXT_H
#include <sys/types.h>
#include <stddef.h>
#include <sys/stat.h>
int   sys_open(const char *path, int flags, int mode);
ssize_t sys_read(int fd, void *buf, size_t count);
ssize_t sys_write(int fd, const void *buf, size_t count);
int   sys_close(int fd);
int   sys_chdir(const char *path);
int   sys_getdents64(int fd, void *dirp, size_t count);
int   sys_stat(const char *path, void *statbuf);
void  sys_exit(int status);
#endif
