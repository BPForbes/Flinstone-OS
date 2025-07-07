#include "interpreter.h"
#include "memory.h"
#include "common.h"
#include <fcntl.h>
#include <unistd.h>
#include "profile.h"
#include "task.h"
#include "util.h"
#include "disk.h"
#include "cluster.h"
#include "fs.h"
#include "threadpool.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int execute_command_str(const char *line) {
    task_gc();
    if (!line || !*line) return 0;
    append_history(line);

    char *cmdLine = strdup(line);

    // I/O Redirection
    int saved_stdin = dup(0), saved_stdout = dup(1);
    char *outp = strchr(cmdLine, '>');
    if (outp) {
        *outp = '\0';
        char *fname = trim_whitespace(outp+1);
        int fd = open(fname, O_CREAT|O_WRONLY|O_TRUNC, 0644);
        if (fd < 0) perror("open");
        else { dup2(fd, 1); close(fd); }
    }
    char *inp = strchr(cmdLine, '<');
    if (inp) {
        *inp = '\0';
        char *fname = trim_whitespace(inp+1);
        int fd = open(fname, O_RDONLY);
        if (fd < 0) perror("open");
        else { dup2(fd, 0); close(fd); }
    }

    char *buf = strdup(line);
    char *trimmed = trim_whitespace(buf);

    char *args[16];
    int argc = 0;
    char *tok = strtok(trimmed, " ");
    while (tok && argc < 15) {
        args[argc++] = tok;
        tok = strtok(NULL, " ");
    }
    args[argc] = NULL;

    if (argc > 0) {
        if (strcmp(args[0], "wait") == 0) {
            if (argc != 2) {
                printf("Usage: wait <task_id>\n");
            } else {
                int id = atoi(args[1]);
                task_wait(id);
            }
            free(cmdLine);
            return 0;
        }

        
        if (strcmp(args[0], "adduser") == 0) {
            if (!has_privilege()) {
                printf("Error: insufficient privileges\n");
            } else {
                /* parse flags -u <name> -p <password> [-root <0|1>] [-rp <rootpass>] */
                char *uname = NULL, *pwd = NULL;
                int is_root_flag = 0;
                char *rp = NULL;
                for (int i = 1; i < argc; i++) {
                    if (strcmp(args[i], "-u") == 0 && i+1 < argc) uname = args[++i];
                    else if (strcmp(args[i], "-p") == 0 && i+1 < argc) pwd = args[++i];
                    else if (strcmp(args[i], "-root") == 0 && i+1 < argc) is_root_flag = atoi(args[++i]);
                    else if (strcmp(args[i], "-rp") == 0 && i+1 < argc) rp = args[++i];
                }
                if (!uname || !pwd) {
                    printf("Usage: adduser -u <name> -p <password> [-root <0|1>] [-rp <rootpass>]\n");
                } else {
                    if (is_root_flag) {
                        if (!rp || strcmp(rp, profiles[0].password) != 0) {
                            printf("Error: invalid root password for elevated user\n");
                            free(cmdLine);
                            return 0;
                        }
                    }
                    if (add_profile(uname, pwd, is_root_flag) == 0)
                        printf("Profile %s added\n", uname);
                    else
                        printf("Failed to add profile %s\n", uname);
                }
            }
            free(cmdLine);
            return 0;
        }

        if (strcmp(args[0], "su") == 0) {
            if (argc < 2) {
                printf("Usage: su <username> [password]\n");
            } else {
                char *target = args[1];
                int idx = -1;
                for (int i = 0; i < profile_count; i++)
                    if (strcmp(profiles[i].name, target)==0) idx = i;
                if (idx < 0) {
                    printf("No such user: %s\n", target);
                } else {
                    if (profiles[idx].is_root) {
                        if (argc < 3 || strcmp(args[2], profiles[idx].password)!=0) {
                            printf("Password required or incorrect for root\n");
                        } else {
                            select_profile(target);
                            printf("Switched to %s\n", target);
                        }
                    } else {
                        select_profile(target);
                        printf("Switched to %s\n", target);
                    }
                }
            }
            free(cmdLine);
            return 0;
        }

        if (strcmp(args[0], "login") == 0) {
            if (argc < 2) printf("Usage: login <root|guest>\n");
            else if (strcmp(args[1], "root")==0) { current_profile=PROFILE_ROOT; printf("Logged in as root\n"); }
            else if (strcmp(args[1], "guest")==0) { current_profile=PROFILE_GUEST; printf("Logged in as guest\n"); }
            else printf("Unknown profile: %s\n", args[1]); free(cmdLine); return 0;
        }
        if (strcmp(args[0], "whoami")==0) { printf("%s\n", current_profile==PROFILE_ROOT?"root":"guest"); free(cmdLine); return 0; }
        if (argc>1 && strcmp(args[argc-1],"&")==0) {
            if (!has_privilege()) printf("Error: insufficient privileges\n");
            else { char buf[1024]; strncpy(buf, line, sizeof(buf)); buf[strlen(buf)-2]=0; int tid=task_submit(buf); printf("[Task %d submitted]\n",tid); }
            free(cmdLine); return 0;
        }
        if (strcmp(args[0],"tasks")==0) { task_list(); free(cmdLine); return 0; }
        if (strcmp(args[0],"kill")==0) { if(argc<2) printf("Usage: kill <id>\n"); else { int id=atoi(args[1]); if(task_kill(id)==0) printf("Task %d killed\n",id); else printf("No such task: %d\n",id); } free(cmdLine); return 0; }

        /* Exit */
        if (strcmp(args[0], "exit")==0) {
            shell_running = 0;
            free(cmdLine);
            return 0;
        }
        /* FS commands */
        if (strcmp(args[0], "cd")==0) {
            if (argc < 2) {
                printf("Usage: cd <directory>\n");
                free(cmdLine);
                return 1;
            }
            if (change_directory(args[1]) < 0)
                printf("Directory not found: %s\n", args[1]);
            free(cmdLine);
            return 0;
        }
        if (strcmp(args[0], "dir")==0) {
            list_directory();
            free(cmdLine);
            return 0;
        }
        if (strcmp(args[0], "pwd")==0) {
            print_working_directory();
            free(cmdLine);
            return 0;
        }
    }

    /* Dash-prefixed commands */
    if (argc > 0 && args[0][0]=='-') {
        /* Make directory in FS */
        if (!has_privilege()) { printf("Error: insufficient privileges\n"); free(cmdLine); return 1; } if (strcmp(args[0], "-D")==0) {
            if (argc < 2) {
                printf("Usage: -D <directory>\n");
                free(cmdLine);
                return 1;
            }
            if (fs_mkdir(args[1]) != 0) {
                printf("Cannot create directory: %s\n", args[1]);
                free(cmdLine);
                return 1;
            }
            free(cmdLine);
            return 0;
        }
        /* Remove directory from FS */
        if (!has_privilege()) { printf("Error: insufficient privileges\n"); free(cmdLine); return 1; } if (strcmp(args[0], "-R")==0) {
            if (argc < 2) {
                printf("Usage: -R <directory>\n");
                free(cmdLine);
                return 1;
            }
            if (fs_rmdir(args[1]) != 0) {
                printf("Cannot remove directory: %s\n", args[1]);
                free(cmdLine);
                return 1;
            }
            free(cmdLine);
            return 0;
        }
        /* Create disk */
        if (strcmp(args[0], "-mkdisk")==0) {
            if (argc < 4) {
                printf("Usage: -mkdisk <volume> <rowCount> <nibbleCount> [<interactive>]\n");
                free(cmdLine);
                return 1;
            }
            int rows   = atoi(args[2]);
            int nibbles= atoi(args[3]);
            if (rows<=0 || nibbles<=0 || (nibbles%2)!=0) {
                printf("Error: rowCount>0 and nibbleCount>0 even.\n");
                free(cmdLine);
                return 1;
            }
            flintstone_format_disk(args[1], rows, nibbles);
            snprintf(current_disk_file, sizeof(current_disk_file),
                     "%s_disk.txt", args[1]);
            g_cluster_size   = nibbles/2;
            g_total_clusters = rows;
            print_disk_formatted();
            free(cmdLine);
            return 0;
        }
        /* Initialize in-memory disk geometry */
        if (strcmp(args[0], "-init")==0) {
            if (argc < 3) {
                printf("Usage: -init <cluster_count> <cluster_size>\n");
                free(cmdLine);
                return 1;
            }
            int c = atoi(args[1]);
            int s = atoi(args[2]);
            if (c<=0||s<=0) {
                printf("Invalid geometry.\n");
                free(cmdLine);
                return 1;
            }
            g_total_clusters = c;
            g_cluster_size   = s;
            printf("Soft init: clusters=%d, size=%d bytes (in memory only).\n",
                   g_total_clusters, g_cluster_size);
            free(cmdLine);
            return 0;
        }
        /* ... other “-” commands unchanged ... */
    }

    printf("Unknown command: %s\n", line);
    free(cmdLine);
    return 1;
}

void interactive_shell(void) {
    /* (Unchanged: loop reading lines, call execute_command_str) */
}

#include <sys/socket.h>
#include <netinet/ip_icmp.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

unsigned short checksum(void *b, int len) {
    unsigned short *buf = b;
    unsigned int sum = 0;
    for (; len > 1; len -= 2) sum += *buf++;
    if (len == 1) sum += *(unsigned char*)buf;
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    return ~sum;
}

void do_ping(const char *ip) {
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0) { perror("socket"); return; }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &addr.sin_addr);

    struct icmphdr icmp;
    memset(&icmp, 0, sizeof(icmp));
    icmp.type = ICMP_ECHO;
    icmp.un.echo.id = getpid();
    icmp.checksum = checksum(&icmp, sizeof(icmp));

    sendto(sockfd, &icmp, sizeof(icmp), 0, (struct sockaddr*)&addr, sizeof(addr));
    printf("ICMP echo sent to %s\n", ip);
    close(sockfd);
}

void do_http_get(const char *host) {
    struct hostent *he = gethostbyname(host);
    if (!he) { perror("gethostbyname"); return; }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(80),
        .sin_addr = *(struct in_addr*)he->h_addr
    };

    if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("connect"); close(sockfd); return;
    }

    char req[256];
    snprintf(req, sizeof(req), "GET / HTTP/1.0\r\nHost: %s\r\n\r\n", host);
    send(sockfd, req, strlen(req), 0);

    char buf[512];
    int n;
    while ((n = read(sockfd, buf, sizeof(buf)-1)) > 0) {
        buf[n] = 0;
        printf("%s", buf);
    }
    close(sockfd);
}


    if (strncmp(line, "ping ", 5) == 0) {
        do_ping(line + 5);
        return 0;
    }
    if (strncmp(line, "httpget ", 8) == 0) {
        do_http_get(line + 8);
        return 0;
    }


#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

void shell_ping(const char* ip) {
    extern void icmp_send_echo(const char* ip);
    icmp_send_echo(ip);
}

void shell_tcpconnect(const char* ip, const char* port_str) {
    int port = atoi(port_str);
    int s = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(port)
    };
    inet_pton(AF_INET, ip, &addr.sin_addr);
    if (connect(s, (struct sockaddr*)&addr, sizeof(addr)) == 0) {
        printf("[TCP] Connected to %s:%d\n", ip, port);
    } else {
        perror("[TCP] connect");
    }
    close(s);
}

void shell_wget(const char* host) {
    extern void do_http_get(const char* host);
    do_http_get(host);
}

int execute_command_str(const char *line) {
    if (strncmp(line, "ping ", 5) == 0) {
        shell_ping(line + 5);
        return 0;
    }
    if (strncmp(line, "tcpconnect ", 11) == 0) {
        const char* args = line + 11;
        char ip[64], port[16];
        sscanf(args, "%63s %15s", ip, port);
        shell_tcpconnect(ip, port);
        return 0;
    }
    if (strncmp(line, "wget ", 5) == 0) {
        shell_wget(line + 5);
        return 0;
    }
