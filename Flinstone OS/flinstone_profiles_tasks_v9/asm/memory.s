/* Simple bump-pointer heap using the linker-provided _end symbol */
.section .bss
.align 8
heap_end:
    .quad 0

.extern _end

.text
.global sbrk
sbrk:
    mov rax, [rip + heap_end]
    test rax, rax
    jne .have_base
    lea rax, [_end]
.have_base:
    mov rbx, rax
    add rax, rdi
    mov [rip + heap_end], rax
    mov rax, rbx
    ret

.global malloc
malloc:
    push rbp
    mov rbp, rsp
    mov rax, rdi
    add rax, 15
    and rax, -16
    mov rdi, rax
    call sbrk
    pop rbp
    ret

.global free
free:
    ret
