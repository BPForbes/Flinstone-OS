.text
.p2align 4

.global strlen
strlen:
    xor    rax, rax
1:  cmp    byte ptr [rdi+rax], 0
    je     2f
    inc    rax
    jmp    1b
2:  ret

.global strcpy
strcpy:
    push   rdi
1:  mov    al, [rsi]
    mov    [rdi], al
    inc    rsi
    inc    rdi
    test   al, al
    jnz    1b
    pop    rax
    ret

.global strcmp
strcmp:
1:  mov    al, [rdi]
    mov    bl, [rsi]
    cmp    al, bl
    jne    2f
    test   al, al
    je     3f
    inc    rdi
    inc    rsi
    jmp    1b
2:  movzx  eax, al
    movzx  edx, bl
    sub    eax, edx
    ret
3:  xor    eax, eax
    ret

.global memcpy
memcpy:
    test   rdx, rdx
    je     2f
1:  mov    al, [rsi]
    mov    [rdi], al
    inc    rsi
    inc    rdi
    dec    rdx
    jnz    1b
2:  lea    rax, [rdi-rcx]
    ret

.global memset
memset:
    mov    al, sil
1:  test   rdx, rdx
    je     2f
    mov    [rdi], al
    inc    rdi
    dec    rdx
    jnz    1b
2:  lea    rax, [rdi-rcx]
    ret

.global memcmp
memcmp:
1:  test   rdx, rdx
    je     3f
    mov    al, [rdi]
    mov    bl, [rsi]
    cmp    al, bl
    jne    2f
    inc    rdi
    inc    rsi
    dec    rdx
    jmp    1b
2:  movzx  eax, al
    movzx  edx, bl
    sub    eax, edx
    ret
3:  xor    eax, eax
    ret
