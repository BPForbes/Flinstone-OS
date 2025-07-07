#ifndef FS_H
#define FS_H

#include <stddef.h>

#define MAX_NAME_LEN 64
#define MAX_CHILDREN 64

typedef enum { FILE_NODE, DIRECTORY_NODE } NodeType;

typedef struct Node {
    char name[MAX_NAME_LEN];
    NodeType type;
    struct Node* parent;
    struct Node* children[MAX_CHILDREN];
    int child_count;
    char *data;
    size_t size;
} Node;

/* Initialize and clean up the in-memory filesystem */
void fs_init(void);
void fs_cleanup(void);

/* Directory operations */
int change_directory(const char *path);
void list_directory(void);
void print_working_directory(void);
int fs_mkdir(const char *name);
int fs_rmdir(const char *name);

#endif /* FS_H */
