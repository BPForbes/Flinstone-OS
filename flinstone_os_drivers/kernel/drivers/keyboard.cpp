// kernel/drivers/keyboard.c
#include <stdint.h>
#include "kernel.h"

#define KBD_STATUS_PORT 0x64
#define KBD_DATA_PORT   0x60

static const char kbd_scancode_map[128] = {
    [0x02] = '1',[0x03] = '2',[0x04] = '3',[0x05] = '4',
    [0x06] = '5',[0x07] = '6',[0x08] = '7',[0x09] = '8',
    [0x0A] = '9',[0x0B] = '0',[0x10] = 'q',[0x11] = 'w',
    [0x12] = 'e',[0x13] = 'r',[0x14] = 't',[0x15] = 'y',
    [0x16] = 'u',[0x17] = 'i',[0x18] = 'o',[0x19] = 'p',
    [0x1E] = 'a',[0x1F] = 's',[0x20] = 'd',[0x21] = 'f',
    [0x22] = 'g',[0x23] = 'h',[0x24] = 'j',[0x25] = 'k',
    [0x26] = 'l',[0x2C] = 'z',[0x2D] = 'x',[0x2E] = 'c',
    [0x2F] = 'v',[0x30] = 'b',[0x31] = 'n',[0x32] = 'm',
    [0x1C] = '\n', [0x39] = ' '
};

static inline uint8_t inb(uint16_t port){
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void keyboard_init(void){
    // Nothing special for polled mode
}

char keyboard_getchar(void){
    uint8_t status, sc;
    do {
        status = inb(KBD_STATUS_PORT);
    } while (!(status & 1));
    sc = inb(KBD_DATA_PORT);
    if (sc < 128) {
        char c = kbd_scancode_map[sc];
        return c ? c : 0;
    }
    return 0;
}