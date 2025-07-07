// kernel/drivers/vga.c
#include "kernel.h"
#include <stddef.h>

static size_t cursor_pos = 0;

void vga_clear() {
    for (size_t i = 0; i < 80*25*2; i += 2) {
        VGA_BUFFER[i] = ' ';
        VGA_BUFFER[i+1] = 0x07;
    }
    cursor_pos = 0;
}

void vga_write_char(char c) {
    VGA_BUFFER[cursor_pos*2] = c;
    VGA_BUFFER[cursor_pos*2+1] = 0x07;
    cursor_pos++;
}

void vga_write_string(const char* s) {
    while (*s) vga_write_char(*s++);
}