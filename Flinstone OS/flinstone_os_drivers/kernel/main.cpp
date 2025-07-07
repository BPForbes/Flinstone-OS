// kernel/main.c
#include "kernel.h"
#include "drivers/keyboard.h"
#include "drivers/vga.h"

void kmain(void) {
    vga_clear();
    vga_write_string("Flinstone Kernel Loaded!\n");
    keyboard_init();
    while (1) {
        char c = keyboard_getchar();
        if (c) vga_write_char(c);
    }
}