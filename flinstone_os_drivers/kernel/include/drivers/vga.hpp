// kernel/include/drivers/vga.h
#ifndef VGA_H
#define VGA_H

void vga_clear(void);
void vga_write_char(char c);
void vga_write_string(const char* s);

#endif