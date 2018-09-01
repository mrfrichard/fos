%include "sconst.inc"

extern gdtPtr
extern cstart
extern idtPtr
extern exception_handler
extern spurious_irq
extern p_proc_ready
extern _main
extern tss
extern disp_str
extern delay
extern k_reenter
extern clock_handler
extern irq_table
extern sys_call_table

[section .bss]
StackSpace  resb 2 * 1024
StackTop:                       ;

[section .text]
[BITS 64]
global _start
global divide_error
global single_step_exception
global nmi
global breakpoint_exception
global overflow
global bounds_check
global inval_opcode
global copr_not_available
global double_fault
global copr_seg_overrun
global inval_tss
global segment_not_present
global stack_exception
global general_protection
global page_fault
global copr_error
global math_fault
global alignment_check
global machine_check
global simd_fault
global virtualization_exception

global hwint00
global hwint01
global hwint02
global hwint03
global hwint04
global hwint05
global hwint06
global hwint07
global hwint08
global hwint09
global hwint10
global hwint11
global hwint12
global hwint13
global hwint14
global hwint15
global sys_call
global restart

_start:
    mov rsp, StackTop
    mov rax, gdtPtr
    sgdt [rax]
    call cstart
    mov rax, gdtPtr
    lgdt [rax]
    lidt [rel idtPtr]

    mov rax, SELECTOR_FLAT_RW
    mov ds, rax
    mov es, rax
    mov ss, rax
    mov fs, rax
    mov rax, SELECTOR_VEDIO
    mov gs, rax
    
csinit:

    sti
    xor rax, rax
    mov rax, SELECTOR_TSS
    ltr ax
    jmp _main

divide_error:
    mov rsi, -1
    mov rdi, 0
    jmp exception
single_step_exception:
    mov rsi, -1
    mov rdi, 1
    jmp exception
nmi:
    mov rsi, -1
    mov rdi, 2
    jmp exception
breakpoint_exception:
    mov rsi, -1
    mov rdi, 3
    jmp exception
overflow:
    mov rsi, -1
    mov rdi, 4
    jmp exception
bounds_check:
    mov rsi, -1
    mov rdi, 5
    jmp exception
inval_opcode:
    mov rsi, -1
    mov rdi, 6
    jmp exception
copr_not_available:
    mov rsi, -1
    mov rdi, 7
    jmp exception
double_fault:
    pop rsi
    mov rdi, 8
    jmp exception
copr_seg_overrun:
    mov rsi, -1
    mov rdi, 9
    jmp exception
inval_tss:
    pop rsi
    mov rdi, 10
    jmp exception
segment_not_present:
    pop rsi
    mov rdi, 11
    jmp exception
stack_exception:
    pop rsi
    mov rdi, 12
    jmp exception
general_protection:
    pop rsi
    mov rdi, 13
    jmp exception
page_fault:
    pop rsi
    mov rdi, 14
    jmp exception
copr_error:
    mov rsi, -1
    mov rdi, 15
    jmp exception
math_fault:
    mov rsi, -1
    mov rdi, 16
    jmp exception
alignment_check:
    mov rsi, -1
    mov rdi, 17
    jmp exception
machine_check:
    mov rsi, -1
    mov rdi, 18
    jmp exception
simd_fault:
    mov rsi, -1
    mov rdi, 19
    jmp exception
virtualization_exception:
    mov rsi, -1
    mov rdi, 20
exception:
    pop rdx
    pop rcx
    pop r8
    call exception_handler
    hlt

%macro hwint_master 1
    call save

    in al, INT_M_CTLMASK
    or al, (1 << %1)
    out INT_M_CTLMASK, al
    
    mov al, EOI
    out INT_M_CTL, al
    sti
    push %1
    call [rel irq_table + 8 * %1]
    pop rcx
    cli

    in al, INT_M_CTLMASK
    and al, ~(1 << %1)
    out INT_M_CTLMASK, al

    ret
%endmacro

hwint00:            ; Interrupt routine for rq 0 (the clock)
    hwint_master 0
hwint01:            ; Interrupt routine for rq 1 (keyboard)
    hwint_master 1
hwint02:            ; Interrupt routine for rq 2 (cascade!)
    hwint_master 2
hwint03:            ; Interrupt routine for rq 3 (second serial)
    hwint_master 3
hwint04:            ; Interrupt routine for rq 4 (first serial)
    hwint_master 4
hwint05:            ; Interrupt routine for rq 5 (XT winchester)
    hwint_master 5
hwint06:            ; Interrupt routine for rq 6 (floppy)
    hwint_master 6
hwint07:            ; Interrupt routine for rq 7 (printer)
    hwint_master 7

%macro hwint_slave 1
    call save

    in al, INT_S_CTLMASK
    or al, (1 << %1)
    out INT_S_CTLMASK, al
    
    mov al, EOI
    out INT_S_CTL, al
    sti
    push %1
    call [rel irq_table + (%1 + 8) * 8]
    pop rcx
    cli

    in al, INT_S_CTLMASK
    and al, ~(1 << %1)
    out INT_S_CTLMASK, al

    ret
%endmacro

hwint08:            ; Interrupt routine for rq 8 ()
    hwint_slave 0
hwint09:            ; Interrupt routine for rq 9 ()
    hwint_slave 1
hwint10:            ; Interrupt routine for rq 10 ()
    hwint_slave 2
hwint11:            ; Interrupt routine for rq 11 ()
    hwint_slave 3
hwint12:            ; Interrupt routine for rq 12 ()
    hwint_slave 4
hwint13:            ; Interrupt routine for rq 13 ()
    hwint_slave 5
hwint14:            ; Interrupt routine for rq 14 ()
    hwint_slave 6
hwint15:            ; Interrupt routine for rq 15 ()
    hwint_slave 7

;sys_call:
;    call save
;    push qword[p_proc_ready]
;    sti
;    push rcx
;    push rbx
;    call [sys_call_table + eax * 4]
;    add rsp, 4 * 3
;    mov [rsi + EAXREG - P_STACKBASE], rax

;    cli
;    ret

save:
    push rax
    push rcx
    push rdx
    push rbx
    push rbp
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
    push fs
    push gs
    
    mov dx, ss
    mov ds, dx
    mov es, dx

    mov rsi, rsp                ; eax = 鏉╂稓鈻肩悰銊ㄦ崳婵婀撮崸锟�
    inc dword [rel k_reenter]
    cmp dword [rel k_reenter], 0
    jne .1

    mov rsp, StackTop           ; 閸掑洦宕查崚鏉垮敶閺嶅憡鐖�

    mov rax, restart
    push rax
    jmp [rsi + RETADR - P_STACKBASE]
.1: ; 娑擃厽鏌囬柌宥呭弳
    mov rax, restart_reenter
    push rax
    jmp [rsi + RETADR - P_STACKBASE]
    
restart:
    mov rsp, [rel p_proc_ready]
    mov rax, qword [rsp + PML4EADDR]
    mov rax, qword [rax]
    mov cr3, rax
    lea rax, [rsp + P_STACKTOP]
    mov qword [rel tss + TSS3_S_SP0], rax
restart_reenter:
    dec dword [rel k_reenter]
    add rsp, 8
    pop gs
    pop fs
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rbp
    pop rbx
    pop rdx
    pop rcx
    pop rax
    add rsp, 8
    iretq

[section .data]
clock_int_msg   db "^", 0
