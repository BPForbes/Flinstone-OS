int real_fs = 0;
#include "common.h"
#include "profile.h"
#include "task.h"

int g_cluster_size = 32;
int g_total_clusters = 32;
char current_disk_file[64] = "drive.txt";

int shell_running = 1;

struct termios orig_termios;

pthread_mutex_t history_mutex = PTHREAD_MUTEX_INITIALIZER;

const char *HELP_MSG =
"Flinstone Shell Commands:\n"
"  -mkdisk <volume> <rowCount> <nibbleCount> [<interactive>]\n"
"       Create a new disk file (<volume>_disk.txt).\n"
"  -init <cluster_count> <cluster_size>\n"
"       Initialize in-memory disk geometry (no file written).\n"
"  -D <directory>\n"
"       Create a directory in the Flinstone FS.\n"
"  -R <directory>\n"
"       Remove a directory from the Flinstone FS.\n"
"  -l\n"
"       List disk clusters (except header).\n"
"  -s <searchtext> [ -t|-h ]\n"
"       Search the disk for <searchtext>.\n"
"  -w <cluster_index> <-t|-h> <data>\n"
"       Write <data> to the specified cluster.\n"
"  -du [-dtl]\n"
"       Disk usage summary or detail.\n"
"  dir\n"
"       List entries in the current Flinstone directory.\n"
"  cd <path>\n"
"       Change Flinstone current directory.\n"
"  pwd\n"
"       Print current Flinstone working directory.\n"
"  help | -h | -?\n"
"       Display this help message.\n"
"  exit\n"
"       Exit the shell.\n";
