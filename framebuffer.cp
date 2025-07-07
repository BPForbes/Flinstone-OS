
#include <stdint.h>
#include <stdio.h>

#define WIDTH  800
#define HEIGHT 600
uint32_t framebuffer[WIDTH * HEIGHT];

void putpixel(int x, int y, uint32_t color) {
    if (x >= 0 && y >= 0 && x < WIDTH && y < HEIGHT) {
        framebuffer[y * WIDTH + x] = color;
    }
}

void fb_clear(uint32_t color) {
    for (int i = 0; i < WIDTH * HEIGHT; i++) framebuffer[i] = color;
    printf("[FB] Framebuffer cleared with color 0x%08X\n", color);
}
