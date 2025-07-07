// kernel/drivers/speaker.c
#include <stdint.h>
#include "kernel.h"

static inline void outb(uint16_t port, uint8_t val){
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

void speaker_beep(uint32_t freq){
    if(freq == 0){
        // turn off speaker
        uint8_t tmp = 0;
        __asm__ volatile("inb $0x61, %0" : "=a"(tmp));
        tmp &= ~0x03;
        outb(0x61, tmp);
        return;
    }
    uint32_t divisor = 1193180 / freq;
    outb(0x43, 0xB6);
    outb(0x42, divisor & 0xFF);
    outb(0x42, (divisor >> 8) & 0xFF);
    uint8_t tmp = 0;
    __asm__ volatile("inb $0x61, %0" : "=a"(tmp));
    tmp |= 0x03;
    outb(0x61, tmp);
}