#include "common.h"
#include "interpreter.h"
#include "threadpool.h"
#include "fs.h"

#include "net/pci.h"
#include "net/nic_e1000.h"

int main(int argc, char **argv) {
    pthread_mutex_init(&history_mutex, NULL);
    fs_init();
    task_init();        /* initialize in-memory FS */

    /* initialize thread pool, history file, etc. (unchanged) */

    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            execute_command_str(argv[i]);
        }
    } else {
        interactive_shell();
    }

    fs_cleanup();
    /* clean up threads and mutexes (unchanged) */
    return 0;
}
