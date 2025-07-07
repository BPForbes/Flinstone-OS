CC = gcc
CFLAGS = -Wall -Wextra -pthread -Iinclude

ASM_SRCS = asm/syscalls_ext.s asm/memory.s asm/string_mem.s
ASM_OBJS = $(ASM_SRCS:.s=.o)

C_SRCS = src/fs_real.c src/fs.c src/common.c profile.c task.c src/util.c src/terminal.c src/interpreter.c src/main.c
C_OBJS = $(C_SRCS:.c=.o)

OBJS = $(ASM_OBJS) $(C_OBJS)

TARGET = flinstone_realfs

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.s
	$(CC) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
