[SECTION .text]
[BITS 64]

global memcpy
global memset
global strcpy

memcpy:
    cld
    mov rax, rdx
    xor rdx, rdx
    mov rcx, 8
    div rcx
    mov rcx, rax
    rep movsq
    cmp rdx, 0
    jz .e
    mov rcx, rdx
    rep movsb
.e:
    ret

memset:
    cld
    mov rax, rsi
    mov ah, al
    push ax
    push ax
    push ax
    push ax
    pop r8
    mov rax, rdx
    xor rdx, rdx
    mov rcx, 8
    div rcx
    mov rcx, rax
    mov rax, r8
    rep stosq
    cmp rdx, 0
    jz .e
    mov rcx, rdx
    rep stosb
.e:
    ret

strcpy:
    mov rcx, -1
    cld
.loop:
    lodsb
    stosb
    inc rcx
    test al, al
    jnz .loop
    mov rax, rcx
    ret