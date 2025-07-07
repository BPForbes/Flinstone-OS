#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <pthread.h>
#include <termios.h>

#define VERSION                "0.02"
#define HISTORY_FILE           "shell_history.txt"
#define NUM_WORKERS            4
#define MAX_JOBS               64

/* Global disk parameters */
extern int g_cluster_size;
extern int g_total_clusters;
extern char current_disk_file[64];

/* Shell control */
extern int shell_running;

/* Terminal settings */
extern struct termios orig_termios;

/* History globals */
extern pthread_mutex_t history_mutex;

/* Help message */
extern const char *HELP_MSG;

#endif /* COMMON_H */
