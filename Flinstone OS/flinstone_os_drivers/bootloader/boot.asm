; bootloader: boot.asm
; Assemble with: nasm -f bin boot.asm -o bootloader/boot.bin

[BITS 16]
[ORG 0x7C00]

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax

    ; Load Kernel (assume at sector 2+, LBA)
    mov ah, 0x02        ; BIOS Read Sectors
    mov al, 5           ; Number of sectors
    mov ch, 0           ; Cylinder 0
    mov cl, 2           ; Sector 2
    mov dh, 0           ; Head 0
    mov dl, [BOOT_DRIVE]
    mov bx, 0x0000
    mov es, bx
    mov bx, 0x0000
    int 0x13

    ; Jump to Kernel entry (0x1000:0000)
    jmp 0x1000:0x0000

; Data
BOOT_DRIVE: db 0

times 510-($-$$) db 0
dw 0xAA55