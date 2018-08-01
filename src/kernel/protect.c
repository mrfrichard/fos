#include "protect.h"
#include "type.h"
#include "global.h"
#include "proto.h"
#include "i8259.h"
#include "string.h"

EXTERN int disp_pos;

PUBLIC void exception_handler (
    T64 vec_no
    , T64 err_code
    , T64 rip
    , T64 cs
    , T64 eflags
    ) {
    int text_color = 0x74;       /* 灰底红字 */

    char err_description[][64] = {
        "#DE Divide Error"
        , "#DB RESERVED"
        , "- NMI Interrupt"
        , "#BP Breakpoint"
        , "#OF Overflow"
        , "#BR BOUND Range Exeeded"
        , "#UD Invalid Opcode (Undefined Opcode)"
        , "#NM Device Not Available (No Math Coprocessor)"
        , "#DF Double Fault"
        , "copprocessor SegmentOverrun(reserved)"
        , "#TS Invalid TSS"
        , "#NP Segment Not Present"
        , "#SS Stack-segment Fault"
        , "#GP General Protection"
        , "#PF General Protection"
        , "- (Intel reserved. Do not use.)"
        , "#MF x87 FPU Floating-point Error(Math Fault)"
        , "#AC Alignment Check"
        , "#MC Machine Check"
        , "#XM SIMD Floation-Point Execption"
        , "#VE Virtualization Exception"
    };

    disp_pos = 0;
    for (int i = 0; i < 80 * 5; ++i){
        disp_str(" ");
    }
    disp_pos = 0;

    disp_color_str(text_color, "Excption! --> ");
    disp_color_str(text_color, err_description[vec_no]);
    disp_str("\n\n");
    disp_color_str(text_color, "EFLAGS: ");
    disp_T64(eflags);
    disp_color_str(text_color, "CS: ");
    disp_T64(cs);
    disp_color_str(text_color, "RIP: ");
    disp_T64(rip);

    if (err_code != -1) {
        disp_color_str(text_color, "Error Code:");
        disp_int(err_code);
    }
}

PRIVATE void init_idt_desc(
    T8 vector
    , T8 desc_type
    , IntHandler handler
    // , T64 handler
    , T8 privilege
) {
    Gate* p_gate        = &idt[vector];
    T64 base            = (T64)handler;
    p_gate->offset_low  = base & 0xffff;
    p_gate->selector    = SELECTOR_KERNEL_CS;
    p_gate->dcount      = 0;
    p_gate->attr        = desc_type | privilege;
    p_gate->offset_high = (base >> 16) & 0xffff;
    p_gate->offset_len  = (base >> 32) & 0xffffffff;
    p_gate->type        = 0;
}

/*PRIVATE void init_descriptor(Descriptor* p_desc, T32 base, T32 limit, T16 attribute) {
    p_desc->limit_low           = limit             & 0xffff;
    p_desc->base_low            = base              & 0xffff;
    p_desc->base_mid            = (base >> 16)      & 0xff;
    p_desc->attrl               = attribute         & 0xff;
    p_desc->limit_high_attr2    = ((limit >> 16)    & 0x0f) | ((attribute >> 8) & 0xf0);
    p_desc->base_high           = (base >> 24)      & 0xff;
}*/

PRIVATE void init_descriptor64(Descriptor64* p_desc, T64 base, T32 limit, T16 attribute) {
    p_desc->limit_low           = limit             & 0xffff;
    p_desc->base_low            = base              & 0xffff;
    p_desc->base_mid            = (base >> 16)      & 0xff;
    p_desc->attrl               = attribute         & 0xff;
    p_desc->limit_high_attr2    = ((limit >> 16)    & 0x0f) | ((attribute >> 8) & 0xf0);
    p_desc->base_high           = (base >> 24)      & 0xff;
    p_desc->base_len            = (base >> 32)      & 0xffffffff;
    p_desc->type                = 0;
}

PUBLIC void init_prot() {
    //ticks = 0;
    init_8259A();
    // 全部初始化成中断门（没有陷阱门）
    init_idt_desc(INT_VECTOR_DIVIDE,                    DA_386IGate,    divide_error,               PRIVILEGE_KRNL);
    init_idt_desc(INT_VECTOR_DEBUG,                     DA_386IGate,    single_step_exception,      PRIVILEGE_KRNL);
    init_idt_desc(INT_VECTOR_NMI,                       DA_386IGate,    nmi,                        PRIVILEGE_KRNL);
    init_idt_desc(INT_VECTOR_BREAKPOINT,                DA_386IGate,    breakpoint_exception,       PRIVILEGE_KRNL);
    init_idt_desc(INT_VECTOR_OVERFLOW,                  DA_386IGate,    overflow,                   PRIVILEGE_KRNL);
    init_idt_desc(INT_VECTOR_BOUNDS,                    DA_386IGate,    bounds_check,               PRIVILEGE_KRNL);
    init_idt_desc(INT_VECTOR_INVAL_OP,                  DA_386IGate,    inval_opcode,               PRIVILEGE_KRNL);
    init_idt_desc(INT_VECTOR_COPROC_NOT,                DA_386IGate,    copr_not_available,         PRIVILEGE_KRNL);
    init_idt_desc(INT_VECTOR_DOUBLE_FAULT,              DA_386IGate,    double_fault,               PRIVILEGE_KRNL);
    init_idt_desc(INT_VECTOR_COPROC_SEG,                DA_386IGate,    copr_seg_overrun,           PRIVILEGE_KRNL);
    init_idt_desc(INT_VECTOR_INVAL_TSS,                 DA_386IGate,    inval_tss,                  PRIVILEGE_KRNL);
    init_idt_desc(INT_VECTOR_SEG_NOT,                   DA_386IGate,    segment_not_present,        PRIVILEGE_KRNL);
    init_idt_desc(INT_VECTOR_STACK_FAULT,               DA_386IGate,    stack_exception,            PRIVILEGE_KRNL);
    init_idt_desc(INT_VECTOR_PROTECTION,                DA_386IGate,    general_protection,         PRIVILEGE_KRNL);
    init_idt_desc(INT_VECTOR_PAGE_FAULT,                DA_386IGate,    page_fault,                 PRIVILEGE_KRNL);
    init_idt_desc(INT_VECTOR_COPROC_ERR,                DA_386IGate,    copr_error,                 PRIVILEGE_KRNL);
    init_idt_desc(INT_VECTOR_MATH_FAULT,                DA_386IGate,    math_fault,                 PRIVILEGE_KRNL);
    init_idt_desc(INT_VECTOR_ALIGNMENT_CHECK,           DA_386IGate,    alignment_check,            PRIVILEGE_KRNL);
    init_idt_desc(INT_VECTOR_MACHINE_CHECK,             DA_386IGate,    machine_check,              PRIVILEGE_KRNL);
    init_idt_desc(INT_VECTOR_SIMD_FAULT,                DA_386IGate,    simd_fault,                 PRIVILEGE_KRNL);
    init_idt_desc(INT_VECTOR_VIRTUALIZATION_EXCEPTION,  DA_386IGate,    virtualization_exception,   PRIVILEGE_KRNL);

    for (int i = INT_VECTOR_VIRTUALIZATION_EXCEPTION + 1; i < INT_VECTOR_IRQ0; i++) {
        init_idt_desc(i,                  DA_386IGate,    spurious_irq,                PRIVILEGE_KRNL);
    }
    
    init_idt_desc(INT_VECTOR_IRQ0 + 0,                  DA_386IGate,    hwint00,                PRIVILEGE_KRNL);
    init_idt_desc(INT_VECTOR_IRQ0 + 1,                  DA_386IGate,    hwint01,                PRIVILEGE_KRNL);
    init_idt_desc(INT_VECTOR_IRQ0 + 2,                  DA_386IGate,    hwint02,                PRIVILEGE_KRNL);
    init_idt_desc(INT_VECTOR_IRQ0 + 3,                  DA_386IGate,    hwint03,                PRIVILEGE_KRNL);
    init_idt_desc(INT_VECTOR_IRQ0 + 4,                  DA_386IGate,    hwint04,                PRIVILEGE_KRNL);
    init_idt_desc(INT_VECTOR_IRQ0 + 5,                  DA_386IGate,    hwint05,                PRIVILEGE_KRNL);
    init_idt_desc(INT_VECTOR_IRQ0 + 6,                  DA_386IGate,    hwint06,                PRIVILEGE_KRNL);
    init_idt_desc(INT_VECTOR_IRQ0 + 7,                  DA_386IGate,    hwint07,                PRIVILEGE_KRNL);
    
    for (int i = INT_VECTOR_IRQ0 + 8; i < INT_VECTOR_IRQ8; i++) {
        init_idt_desc(i,                  DA_386IGate,    spurious_irq,                PRIVILEGE_KRNL);
    }
    
    init_idt_desc(INT_VECTOR_IRQ8 + 0,                  DA_386IGate,    hwint08,                PRIVILEGE_KRNL);
    init_idt_desc(INT_VECTOR_IRQ8 + 1,                  DA_386IGate,    hwint09,                PRIVILEGE_KRNL);
    init_idt_desc(INT_VECTOR_IRQ8 + 2,                  DA_386IGate,    hwint10,                PRIVILEGE_KRNL);
    init_idt_desc(INT_VECTOR_IRQ8 + 3,                  DA_386IGate,    hwint11,                PRIVILEGE_KRNL);
    init_idt_desc(INT_VECTOR_IRQ8 + 4,                  DA_386IGate,    hwint12,                PRIVILEGE_KRNL);
    init_idt_desc(INT_VECTOR_IRQ8 + 5,                  DA_386IGate,    hwint13,                PRIVILEGE_KRNL);
    init_idt_desc(INT_VECTOR_IRQ8 + 6,                  DA_386IGate,    hwint14,                PRIVILEGE_KRNL);
    init_idt_desc(INT_VECTOR_IRQ8 + 7,                  DA_386IGate,    hwint15,                PRIVILEGE_KRNL);

    for (int i = INT_VECTOR_IRQ8 + 8; i < 256; i++) {
        init_idt_desc(i,                  DA_386IGate,    spurious_irq,                PRIVILEGE_KRNL);
    }
    
//    init_idt_desc(INT_VECTOR_SYS_CALL,                  DA_386IGate,    sys_call,               PRIVILEGE_USER);

    memset((void*)&tss, 0, sizeof(tss));
    init_descriptor64(
        (Descriptor64*)&gdt[INDEX_TSS]
        , (T64)&tss
        , sizeof(tss)
        , DA_386TSS
    );
} 