[section .data]
[BITS 64]
global disp_pos
disp_pos dq 0
[section .text]

[BITS 64]
global disp_color_str_pos
global disp_color_str
global disp_str
global disp_str_pos
global outb
global inb
global disable_irq
global enable_irq

disp_color_str_pos:
    mov rax, rdi
    mov ah, al
    mov rdi, rdx
.21:
    lodsb
    test al, al
    jz  .22
    cmp al, 0Ah;
    jnz .23
    push rax
    mov rax, rdi
    mov r8b, 160
    div r8b
    and rax, 0FFh
    inc rax
    mov r8b, 160
    mul r8b
    mov rdi, rax
    pop rax
    jmp .21
.23:
    mov [gs:rdi], ax
    add rdi, 2
    jmp .21

.22:
    mov rax, rdi
    ret

disp_color_str:
    mov rdx, [rel disp_pos]
    call disp_color_str_pos
    mov [rel disp_pos], rax
    ret

disp_str:
    mov rsi, rdi
    mov rdi, 0Fh 
    call disp_color_str
    ret

disp_str_pos:
    mov rdx, rsi
    mov rsi, rdi
    mov rdi, 0Fh
    call disp_color_str_pos
    ret

outb:
    mov rax, rsi
    mov rdx, rdi
    out dx, al
    nop
    nop
    nop
    nop
    ret

inb:
    mov rdx, rdi
    xor eax, eax
    in al, dx
    nop
    nop
    nop
    nop
    ret

%include "sconst.inc"

disable_irq:
    mov ecx, edi
    pushf
    cli
    mov ah, 1
    rol ah, cl
    cmp cl, 8
    jae disable_8
disable_0:
    in al, INT_M_CTLMASK
    test al, ah
    jnz dis_already
    or al, ah
    out INT_M_CTLMASK, al
    popf
    mov rax, 1
    ret
disable_8:
    in al, INT_S_CTLMASK
    test al, ah
    jnz dis_already
    or al, ah
    out INT_S_CTLMASK, al
    popf
    mov rax, 1
    ret
dis_already:
    popf
    xor rax, rax
    ret

enable_irq:
    mov ecx, edi
    pushf
    cli
    mov ah, ~1
    rol ah, cl
    cmp cl, 8
    jae enable_8
enable_0:
    in al, INT_M_CTLMASK
    and al, ah
    out INT_M_CTLMASK, al
    popf
    ret
enable_8:
    in al, INT_S_CTLMASK
    and al, ah
    out INT_S_CTLMASK, al
    popf
    ret