/* Low-level block I/O against a memory region “disk_base” */
.section .data
.global disk_base
disk_base:
    .quad 0

.section .data
.global block_size
block_size:
    .quad 64

.text
.global read_block
read_block:
    mov rcx, [rip + block_size]
    mov rax, [rip + disk_base]
    mov rdx, rdi
    mul rcx
    add rax, [rip + disk_base]
.copy_loop:
    mov bl, [rax]
    mov [rsi], bl
    inc rax
    inc rsi
    dec rcx
    jnz .copy_loop
    xor rax, rax
    ret

.global write_block
write_block:
    mov rcx, [rip + block_size]
    mov rax, [rip + disk_base]
    mov rdx, rdi
    mul rcx
    add rax, [rip + disk_base]
.copy_loop2:
    mov bl, [rsi]
    mov [rax], bl
    inc rax
    inc rsi
    dec rcx
    jnz .copy_loop2
    xor rax, rax
    ret
