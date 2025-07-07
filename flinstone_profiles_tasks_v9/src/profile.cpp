#include "profile.h"
#include <string.h>
#include <stdio.h>

Profile profiles[MAX_PROFILES];
int profile_count = 0;
int current_profile_idx = 0;

int add_profile(const char *name, const char *password, int is_root) {
    if (profile_count >= MAX_PROFILES) return -1;
    for (int i = 0; i < profile_count; i++) {
        if (strcmp(profiles[i].name, name) == 0) return -1;
    }
    strncpy(profiles[profile_count].name, name, 63);
    profiles[profile_count].name[63] = '\0';
    strncpy(profiles[profile_count].password, password, 63);
    profiles[profile_count].password[63] = '\0';
    profiles[profile_count].is_root = is_root;
    profile_count++;
    return 0;
}

int del_profile(const char *name) {
    for (int i = 0; i < profile_count; i++) {
        if (strcmp(profiles[i].name, name) == 0) {
            if (i == current_profile_idx) return -1;
            for (int j = i; j < profile_count - 1; j++) {
                profiles[j] = profiles[j+1];
            }
            profile_count--;
            if (current_profile_idx > i) current_profile_idx--;
            return 0;
        }
    }
    return -1;
}

int select_profile(const char *name) {
    for (int i = 0; i < profile_count; i++) {
        if (strcmp(profiles[i].name, name) == 0) {
            current_profile_idx = i;
            return 0;
        }
    }
    return -1;
}

void list_profiles(void) {
    printf("Profiles:\n");
    for (int i = 0; i < profile_count; i++) {
        printf(" %s%s\n", profiles[i].name,
               (i==current_profile_idx) ? " (current)" : "");
    }
}

/* initialize default profiles */
__attribute__((constructor))
static void init_profiles(void) {
    add_profile("root", "root", 1);
    add_profile("guest", "", 0);
    current_profile_idx = 1; /* start as guest */
}
