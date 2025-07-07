.text
.global sys_open
sys_open:
    mov rax, 2
    syscall
    ret

.global sys_read
sys_read:
    mov rax, 0
    syscall
    ret

.global sys_write
sys_write:
    mov rax, 1
    syscall
    ret

.global sys_close
sys_close:
    mov rax, 3
    syscall
    ret

.global sys_chdir
sys_chdir:
    mov rax, 80
    syscall
    ret

.global sys_getdents64
sys_getdents64:
    mov rax, 217
    syscall
    ret

.global sys_stat
sys_stat:
    mov rax, 4
    syscall
    ret

.global sys_exit
sys_exit:
    mov rax, 60
    syscall
    hlt
