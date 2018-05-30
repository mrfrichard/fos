#ifndef _FOS_PROTECT_H_
#define _FOS_PROTECT_H_

#include "type.h"

typedef struct s_descriptor {
	T16 limit_low;
	T16 base_low;
	T8	base_mid;
	T8 attrl;               /* P(1) DPL(2) DT(1) TYPE(4) */
	T8 limit_high_attr2;    /* G(1) D(1) 0(1) AVL(1) LimitHigh(4) */
	T8 base_high;
} Descriptor;

typedef struct s_descriptor64 {
	T16 limit_low;
	T16 base_low;
	T8	base_mid;
	T8 attrl;               /* P(1) DPL(2) DT(1) TYPE(4) */
	T8 limit_high_attr2;    /* G(1) D(1) 0(1) AVL(1) LimitHigh(4) */
	T8 base_high;
    T32 base_len;
    T8  reserve_low;
    T8  type;
    T16 reserve_height;
} Descriptor64;


typedef struct s_gate {
	T16 offset_low;
	T16 selector;
	T8	dcount;
	T8  attr;               /* P(1) DPL(2) DT(1) TYPE(4) */
	T16 offset_high;
    T32 offset_len;
    T8  reserve_low;
    T8  type;
    T16 reserve_height;
} Gate;

typedef struct s_tss {
    T32 blacklink;
    T32 esp0;
    T32 ss0;
    T32 esp1;
    T32 bss1;
    T32 eesp2;
    T32 sss2;
    T32 ecr3;
    T32 eip;
    T32 flags;
    T32 eax;
    T32 ecx;
    T32 edx;
    T32 ebx;
    T32 esp;
    T32 ebp;
    T32 esi;
    T32 edi;
    T32 es;
    T32 cs;
    T32 ss;
    T32 ds;
    T32 fs;
    T32 gs;
    T32 ldt;
    T16 trap;
    T16 iobase;
} Tss;

typedef struct s_tss64 {
    T32 reserved0;
    T64 rsp0;
    T64 rsp1;
    T64 rsp2;
    T64 reserved1;
    T64 ist1;
    T64 ist2;
    T64 ist3;
    T64 ist4;
    T64 ist5;
    T64 ist6;
    T64 ist7;
    T64 reserved2;
    T16 reserved3;
    T16 iobase;
} __attribute__ ((packed)) Tss64;



#define GDT_SIZE (8)
#define IDT_SIZE (256)

/* GDT */
#define DA_LDT                  (0x82)
#define DA_386TSS               (0x89)
#define DA_386CGate             (0x8c)
#define DA_386IGate             (0x8e)
#define DA_DRW                  (0x92)
#define DA_C                    (0x98)
#define DA_CR                   (0x98)

#define INDEX_DUMMY     (0)
#define INDEX_FLAT_C    (1)
#define INDEX_FLAT_RW   (2)
#define INDEX_VIDEO     (3)
#define INDEX_USER_C    (4)
#define INDEX_USER_RW   (5)
#define INDEX_TSS       (6)

#define DA_DPL0                 (0x00)
#define DA_DPL1                 (0x20)
#define DA_DPL2                 (0x40)
#define DA_DPL3                 (0x60)

#define PRIVILEGE_KRNL          (DA_DPL0)
#define PRIVILEGE_TASK          (DA_DPL1)
#define PRIVILEGE_USER          (DA_DPL3)


/* SELECTOR */
#define SA_RPL_MASK (0xfffc)
#define SA_RPL0 (0)
#define SA_RPL1 (1)
#define SA_RPL2 (2)
#define SA_RPL3 (3)

#define SA_TI_MASK (0xfffb)
#define SA_TIG (0)
#define SA_TIL (4)

#define SELECTOR_DUMMY      ( INDEX_DUMMY                       )
#define SELECTOR_FLAT_C     ( INDEX_FLAT_C      << 3            )
#define SELECTOR_FLAT_RW    ( INDEX_FLAT_RW     << 3            )
#define SELECTOR_VIDEO      ( INDEX_VIDEO       << 3 | SA_RPL3  )
#define SELECTOR_USER_C     ( INDEX_USER_C      << 3 | SA_RPL3  )
#define SELECTOR_USER_RW    ( INDEX_USER_RW     << 3 | SA_RPL3  )
#define SELECTOR_TSS        ( INDEX_TSS         << 3            )

#define SELECTOR_KERNEL_CS  ( SELECTOR_FLAT_C   )
#define SELECTOR_KERNEL_DS  ( SELECTOR_FLAT_RW  )
#define SELECTOR_KERNEL_GS  ( SELECTOR_VIDEO    )

#define RPL_TASK    (SA_RPL1)
#define RPL_USER    (SA_RPL3)

/* 中断向量 */
#define INT_VECTOR_DIVIDE                   (0x00)
#define INT_VECTOR_DEBUG                    (0x01)
#define INT_VECTOR_NMI                      (0x02)
#define INT_VECTOR_BREAKPOINT               (0x03)
#define INT_VECTOR_OVERFLOW                 (0x04)
#define INT_VECTOR_BOUNDS                   (0x05)
#define INT_VECTOR_INVAL_OP                 (0x06)
#define INT_VECTOR_COPROC_NOT               (0x07)
#define INT_VECTOR_DOUBLE_FAULT             (0x08)
#define INT_VECTOR_COPROC_SEG               (0x09)
#define INT_VECTOR_INVAL_TSS                (0x0a)
#define INT_VECTOR_SEG_NOT                  (0x0b)
#define INT_VECTOR_STACK_FAULT              (0x0c)
#define INT_VECTOR_PROTECTION               (0x0d)
#define INT_VECTOR_PAGE_FAULT               (0x0e)
#define INT_VECTOR_COPROC_ERR               (0x0f)
#define INT_VECTOR_MATH_FAULT               (0x10)
#define INT_VECTOR_ALIGNMENT_CHECK          (0x11)
#define INT_VECTOR_MACHINE_CHECK            (0x12)
#define INT_VECTOR_SIMD_FAULT               (0x13)
#define INT_VECTOR_VIRTUALIZATION_EXCEPTION (0x14)

#define INT_VECTOR_IRQ0     (0x20)
#define INT_VECTOR_IRQ8     (0x28)
#define INT_VECTOR_SYS_CALL (0x90)


#define CLOCK_IRQ       0
#define KEY_BOARD_IRQ   1

PUBLIC void init_prot();
PUBLIC T32 seg2phys(T16 seg);

#endif /* _FOS_PROTECT_H_ */
