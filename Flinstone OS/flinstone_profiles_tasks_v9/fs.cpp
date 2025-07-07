#include "fs.h"
#include "memory.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static Node *root = NULL;
static Node *cwd  = NULL;

/* Helper: create a node */
static Node *node_create(const char *name, NodeType type) {
    Node *n = malloc(sizeof(Node));
    if (!n) return NULL;
    strncpy(n->name, name, MAX_NAME_LEN-1);
    n->name[MAX_NAME_LEN-1] = '\0';
    n->type = type;
    n->parent = NULL;
    n->child_count = 0;
    for (int i = 0; i < MAX_CHILDREN; i++) n->children[i] = NULL;
    n->data = NULL;
    n->size = 0;
    return n;
}

/* Recursive free */
static void node_free(Node *n) {
    if (!n) return;
    for (int i = 0; i < n->child_count; i++)
        node_free(n->children[i]);
    free(n->data);
    free(n);
}

void fs_init(void) {
    if (root) return;
    root = node_create("/", DIRECTORY_NODE);
    root->parent = NULL;
    cwd = root;
}

void fs_cleanup(void) {
    node_free(root);
    root = cwd = NULL;
}

/* Split a path into components */
static char **split_path(char *path, int *count) {
    char **comps = malloc(sizeof(char*) * MAX_CHILDREN);
    int idx = 0;
    char *token = strtok(path, "/");
    while (token && idx < MAX_CHILDREN) {
        comps[idx++] = strdup(token);
        token = strtok(NULL, "/");
    }
    *count = idx;
    return comps;
}

/* Traverse from start by components */
static Node *find_node(Node *start, char **comps, int n) {
    Node *cur = start;
    for (int i = 0; i < n && cur; i++) {
        if (strcmp(comps[i], ".")==0) continue;
        if (strcmp(comps[i], "..")==0) {
            if (cur->parent) cur = cur->parent;
        } else {
            Node *next = NULL;
            if (cur->type != DIRECTORY_NODE) return NULL;
            for (int j = 0; j < cur->child_count; j++) {
                if (strcmp(cur->children[j]->name, comps[i])==0) {
                    next = cur->children[j];
                    break;
                }
            }
            if (!next) return NULL;
            cur = next;
        }
    }
    return cur;
}

int change_directory(const char *path) {
    if (!root) fs_init();
    char tmp[1024];
    strncpy(tmp, path, sizeof(tmp)-1);
    tmp[sizeof(tmp)-1] = '\0';

    Node *start = (path[0]=='/') ? root : cwd;
    int count;
    char **comps = split_path(tmp, &count);
    Node *target = find_node(start, comps, count);
    for (int i = 0; i < count; i++) free(comps[i]);
    free(comps);

    if (!target || target->type != DIRECTORY_NODE)
        return -1;
    cwd = target;
    return 0;
}

void list_directory(void) {
    if (!root) fs_init();
    printf("\n");
    for (int i = 0; i < cwd->child_count; i++) {
        Node *c = cwd->children[i];
        if (c->type == DIRECTORY_NODE)
            printf("%s/\t", c->name);
        else
            printf("%s\t", c->name);
    }
    printf("\n");
}

void print_working_directory(void) {
    if (!root) fs_init();
    char path[1024] = "";
    Node *cur = cwd;
    while (cur && cur != root) {
        char buf[1024];
        snprintf(buf, sizeof(buf), "/%s%s", cur->name, path);
        strncpy(path, buf, sizeof(path)-1);
        cur = cur->parent;
    }
    if (path[0]=='\0')
        printf("/\n");
    else
        printf("%s\n", path);
}

int fs_mkdir(const char *name) {
    if (!root) fs_init();
    if (cwd->child_count >= MAX_CHILDREN) return -1;
    /* Check for duplicate */
    for (int i = 0; i < cwd->child_count; i++)
        if (strcmp(cwd->children[i]->name, name)==0)
            return -1;
    Node *dir = node_create(name, DIRECTORY_NODE);
    if (!dir) return -1;
    dir->parent = cwd;
    cwd->children[cwd->child_count++] = dir;
    return 0;
}

int fs_rmdir(const char *name) {
    if (!root) fs_init();
    int idx = -1;
    for (int i = 0; i < cwd->child_count; i++) {
        if (strcmp(cwd->children[i]->name, name)==0 &&
            cwd->children[i]->type == DIRECTORY_NODE) {
            /* only remove if empty */
            if (cwd->children[i]->child_count != 0)
                return -1;
            idx = i;
            break;
        }
    }
    if (idx < 0) return -1;
    Node *to_remove = cwd->children[idx];
    /* shift siblings */
    for (int j = idx; j < cwd->child_count-1; j++)
        cwd->children[j] = cwd->children[j+1];
    cwd->child_count--;
    free(to_remove);
    return 0;
}