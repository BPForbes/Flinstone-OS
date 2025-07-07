#include "util.h"
#include "common.h"
#include <pthread.h>

char *trim_whitespace(char *str) {
    if (!str) return NULL;
    while (isspace((unsigned char)*str)) str++;
    if (!*str) return str;
    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    return str;
}

void append_history(const char *cmd) {
    pthread_mutex_lock(&history_mutex);
    FILE *fp = fopen(HISTORY_FILE, "a");
    if (fp) {
        fprintf(fp, "%s\n", cmd);
        fclose(fp);
    }
    pthread_mutex_unlock(&history_mutex);
}
