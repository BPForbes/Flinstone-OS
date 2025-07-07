#ifndef PROFILE_H
#define PROFILE_H

#include <stddef.h>
#define MAX_PROFILES 64

typedef struct {
    char name[64];
    char password[64];
    int is_root;
} Profile;

extern Profile profiles[MAX_PROFILES];
extern int profile_count;
extern int current_profile_idx;

/* privilege check */
static inline int has_privilege() {
    return profiles[current_profile_idx].is_root;
}

/* profile management */
int add_profile(const char *name, const char *password, int is_root);
int del_profile(const char *name);
int select_profile(const char *name);
void list_profiles(void);

#endif /* PROFILE_H */
