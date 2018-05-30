#ifndef _TINIX_PROC_H_
#define _TINIX_PROC_H_

#include "const.h"
#include "type.h"

typedef struct s_stackframe {
    T64 gs;
    T64 fs;
    T64 r15;
    T64 r14;
    T64 r13;
    T64 r12;
    T64 r11;
    T64 r10;
    T64 r9;
    T64 r8;
    T64 rdi;
    T64 rsi;
    T64 rbp;
//    T64 kernel_esp;
    T64 rbx;
    T64 rdx;
    T64 rcx;
    T64 rax;
    T64 retaddr;
    T64 rip;
    T64 cs;
    T64 eflags;
    T64 rsp;
    T64 ss;
} STACK_FRAME;

typedef struct s_proc {
    STACK_FRAME regs;
    T64         pml4e;
    T32         pid;
    char        p_name[PROC_NAME_LEN];
    int         ticks;
    int         priority;
    int         nr_tty;
} PROCESS;

typedef struct s_task {
    TaskFun     initial_eip;
    int         stacksize;
    char        name[PROC_NAME_LEN];
    int         priority;
}TASK;

#define STACK_SIZE_TESTA (0x8000)
#define STACK_SIZE_TESTB (0x8000)
#define STACK_SIZE_TESTC (0x8000)
#define STACK_SIZE_TTY (0x8000)


#define STACK_SIZE_TOTAL (STACK_SIZE_TESTA + STACK_SIZE_TESTB + STACK_SIZE_TESTC + STACK_SIZE_TTY)

#define NR_TASKS        (0)
#define NR_PROCS        (3)

PUBLIC unsigned int sys_get_ticks();
PUBLIC void schedule();


#endif /* _TINIX_PROC_H_ */
