#ifndef TASK_H
#define TASK_H

void task_init(void);
int task_wait(int id);
void task_gc(void);
int task_submit(const char *cmd);
void task_list(void);
int task_kill(int id);

#endif
