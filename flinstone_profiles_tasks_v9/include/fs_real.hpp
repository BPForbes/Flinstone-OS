#ifndef FS_REAL_H
#define FS_REAL_H
#include <sys/stat.h>
#include <stddef.h>
typedef struct FILE_asm FILE_asm;
typedef struct DIR_asm DIR_asm;
typedef struct dirent_asm dirent_asm;
FILE_asm *fopen_asm(const char *path, const char *mode);
size_t fread_asm(void *ptr, size_t size, size_t nmemb, FILE_asm *f);
size_t fwrite_asm(const void *ptr, size_t size, size_t nmemb, FILE_asm *f);
int fclose_asm(FILE_asm *f);
DIR_asm *opendir_asm(const char *path);
dirent_asm *readdir_asm(DIR_asm *d);
int closedir_asm(DIR_asm *d);
int mkdir_asm(const char *path, mode_t mode);
int rmdir_asm(const char *path);
int stat_asm(const char *path, struct stat *st);
#endif
