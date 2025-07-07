; kernel/drivers/keyboard.asm
[BITS 32]
global keyboard_init
global keyboard_getchar

keyboard_init:
    ; Initialize PS/2 keyboard controller & IRQ1 handler
    ret

keyboard_getchar:
    xor eax, eax
    ; Read scancode from port 0x60 (not implemented)
    ret