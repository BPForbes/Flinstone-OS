#include "fs.h"
#include "memory.h"
#include "common.h"
#include "fs_real.h"
#include <stdio.h>
#include <stdlib.h>

void fs_init(void) {}
void fs_cleanup(void) {}

int fs_mkdir(const char *name) {
    if (real_fs) return mkdir_asm(name, 0755);
    return -1;
}

int fs_rmdir(const char *name) {
    if (real_fs) return rmdir_asm(name);
    return -1;
}

int change_directory(const char *path) {
    if (real_fs) return sys_chdir(path);
    return -1;
}

void list_directory(void) {
    if (real_fs) {
        DIR_asm *d = opendir_asm(".");
        dirent_asm *e;
        while ((e = readdir_asm(d))) {
            printf("%s\t", e->d_name);
            free(e);
        }
        printf("\n");
        closedir_asm(d);
    } else {
        printf("Virtual FS unsupported\n");
    }
}

void print_working_directory(void) {
    if (real_fs) {
        char buf[1024];
        if (getcwd(buf, sizeof(buf))) printf("%s\n", buf);
        else perror("getcwd");
    } else {
        printf("/virtual\n");
    }
}