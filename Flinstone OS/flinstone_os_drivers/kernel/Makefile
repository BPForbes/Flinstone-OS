ASM = nasm
CC = i686-elf-gcc
LD = i686-elf-ld
CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -Iinclude
LDFLAGS = -T linker.ld

SRCS = main.c drivers/vga.c drivers/speaker.c drivers/mouse.c
OBJS = $(SRCS:.c=.o) keyboard.o

all: kernel.bin

keyboard.o: drivers/keyboard.asm
	$(ASM) -f elf32 $< -o keyboard.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

kernel.bin: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

clean:
	rm -f *.o kernel.bin
SRCS += net/icmp.c
SRCS += net/arp.c
SRCS += net/logger.c