#include "task.h"
#include "memory.h"
#include "interpreter.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>

typedef enum { TASK_PENDING, TASK_RUNNING, TASK_DONE, TASK_CANCELLED } task_status_t;

typedef struct {
    int id;
    char *cmd;
    pthread_t thread;
    task_status_t status;
} Task;

static Task **tasks = NULL;
static int task_count = 0;
static int next_task_id = 1;
static pthread_mutex_t task_mutex = PTHREAD_MUTEX_INITIALIZER;

static void *task_wrapper(void *arg) {
    Task *t = arg;
    pthread_mutex_lock(&task_mutex);
    t->status = TASK_RUNNING;
    pthread_mutex_unlock(&task_mutex);
    execute_command_str(t->cmd);
    pthread_mutex_lock(&task_mutex);
    if (t->status != TASK_CANCELLED) t->status = TASK_DONE;
    pthread_mutex_unlock(&task_mutex);
    return NULL;
}

void task_init(void) {
    // nothing
}

int task_submit(const char *cmd) {
    Task *t = malloc(sizeof(Task));
    t->id = next_task_id++;
    t->cmd = strdup(cmd);
    t->status = TASK_PENDING;
    pthread_mutex_lock(&task_mutex);
    tasks = realloc(tasks, sizeof(Task*)*(task_count+1));
    tasks[task_count++] = t;
    pthread_mutex_unlock(&task_mutex);
    pthread_create(&t->thread, NULL, task_wrapper, t);
    return t->id;
}

void task_list(void) {
    pthread_mutex_lock(&task_mutex);
    printf("ID\tStatus\t\tCmd\n");
    for(int i=0;i<task_count;i++){
        Task *t = tasks[i];
        const char *s = "UNKNOWN";
        switch(t->status){
            case TASK_PENDING:   s="PENDING"; break;
            case TASK_RUNNING:   s="RUNNING"; break;
            case TASK_DONE:      s="DONE";    break;
            case TASK_CANCELLED: s="CANCELLED"; break;
        }
        printf("%d\t%-10s\t%s\n", t->id, s, t->cmd);
    }
    pthread_mutex_unlock(&task_mutex);
}

int task_kill(int id) {
    pthread_mutex_lock(&task_mutex);
    for(int i=0;i<task_count;i++){
        Task *t = tasks[i];
        if(t->id == id){
            if(t->status == TASK_PENDING){
                t->status = TASK_CANCELLED
                pthread_mutex_unlock(&task_mutex)
                return 0;
            }
            if(t->status == TASK_RUNNING){
                pthread_cancel(t->thread);
                t->status = TASK_CANCELLED;
                pthread_mutex_unlock(&task_mutex)
                return 0;
            }
            break;
        }
    }
    pthread_mutex_unlock(&task_mutex)
    return -1;
}
int task_wait(int id) {
    for (int i = 0; i < task_count; i++) {
        Task *t = tasks[i];
        if (t->id == id) {
            pthread_join(t->thread, NULL);
            return 0;
        }
    }
    return -1;
}


void task_gc(void) {
    pthread_mutex_lock(&task_mutex);
    int write_idx = 0;
    for (int i = 0; i < task_count; i++) {
        Task *t = tasks[i];
        if (t->status == TASK_DONE || t->status == TASK_CANCELLED) {
            pthread_join(t->thread, NULL);
            free(t->cmd);
            free(t);
        } else {
            tasks[write_idx++] = t;
        }
    }
    task_count = write_idx;
    tasks = realloc(tasks, sizeof(Task*) * task_count);
    pthread_mutex_unlock(&task_mutex);
}
