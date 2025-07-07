// kernel/drivers/mouse.c
#include <stdint.h>
#include "kernel.h"

static inline void outb(uint16_t port, uint8_t val){
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}
static inline uint8_t inb(uint16_t port){
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}
static void wait_input_buffer_empty(){
    while(inb(0x64) & 2);
}
static void wait_output_buffer_full(){
    while(!(inb(0x64) & 1));
}

void mouse_init(void){
    wait_input_buffer_empty();
    outb(0x64, 0xA8); // enable auxiliary device - mouse
    wait_input_buffer_empty();
    outb(0x64, 0xD4); // next byte to mouse
    wait_input_buffer_empty();
    outb(0x60, 0xF4); // enable packet streaming
    wait_output_buffer_full();
    inb(0x60); // ack
}

int mouse_get_event(void){
    // returns 1 if packet available, else 0
    wait_output_buffer_full();
    uint8_t status = inb(0x64);
    if(!(status & 1)) return 0;
    // read 3 bytes packet
    uint8_t packet[3];
    for(int i=0;i<3;i++){
        packet[i] = inb(0x60);
    }
    // combine into event: return packet[0];
    return packet[0];
}