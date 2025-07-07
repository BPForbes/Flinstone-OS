#include "fs_real.h"
#include "memory.h"
#include "syscalls_ext.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <linux/dirent.h>

typedef struct { int fd; } FILE_asm;
typedef struct { int fd; char buf[4096]; size_t buf_sz, buf_off; } DIR_asm;
typedef struct { ino_t d_ino; char d_name[256]; unsigned char d_type; } dirent_asm;

FILE_asm *fopen_asm(const char *path, const char *mode) {
    int flags = 0, perm = 0644;
    if (strcmp(mode,"r")==0) flags = 0;
    else if (strcmp(mode,"w")==0) flags = O_CREAT|O_WRONLY|O_TRUNC;
    else if (strcmp(mode,"a")==0) flags = O_CREAT|O_WRONLY|O_APPEND;
    int fd = sys_open(path, flags, perm);
    if (fd < 0) return NULL;
    FILE_asm *f = malloc(sizeof(*f)); f->fd = fd; return f;
}
size_t fread_asm(void *ptr, size_t size, size_t nmemb, FILE_asm *f) {
    ssize_t r = sys_read(f->fd, ptr, size*nmemb);
    return (r<0)?0:(size_t)r/size;
}
size_t fwrite_asm(const void *ptr, size_t size, size_t nmemb, FILE_asm *f) {
    ssize_t w = sys_write(f->fd, ptr, size*nmemb);
    return (w<0)?0:(size_t)w/size;
}
int fclose_asm(FILE_asm *f) { int r = sys_close(f->fd); free(f); return r; }

DIR_asm *opendir_asm(const char *path) {
    int fd = sys_open(path, O_RDONLY|O_DIRECTORY, 0);
    if (fd<0) return NULL;
    DIR_asm *d = malloc(sizeof(*d)); d->fd = fd; d->buf_sz = d->buf_off = 0; return d;
}
dirent_asm *readdir_asm(DIR_asm *d) {
    while (1) {
        if (d->buf_off >= d->buf_sz) {
            int n = sys_getdents64(d->fd, d->buf, sizeof(d->buf));
            if (n <= 0) return NULL;
            d->buf_sz = n; d->buf_off = 0;
        }
        struct linux_dirent64 *de = (void*)(d->buf + d->buf_off);
        d->buf_off += de->d_reclen;
        dirent_asm *ent = malloc(sizeof(*ent));
        ent->d_ino = de->d_ino; ent->d_type = de->d_type;
        strncpy(ent->d_name, de->d_name, 255); ent->d_name[255] = '\0';
        return ent;
    }
}
int closedir_asm(DIR_asm *d) { int r = sys_close(d->fd); free(d); return r; }
int mkdir_asm(const char *path, mode_t mode) {
    return syscall(83, path, mode);
}
int rmdir_asm(const char *path) {
    return syscall(84, path);
}
int stat_asm(const char *path, struct stat *st) {
    return sys_stat(path, st);
}