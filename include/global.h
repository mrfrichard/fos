#ifndef _FOS_GLOBAL_H_
#define _FOS_GLOBAL_H_

#include "type.h"
#include "protect.h"
#include "const.h"
#include "proc.h"

#ifdef GLOBAL_VARIABLES_HERE
#undef EXTERN
#define EXTERN
#endif

EXTERN T8 gdtPtr[10];
EXTERN Descriptor gdt[GDT_SIZE];
EXTERN T8 idtPtr[10];
EXTERN Gate idt[IDT_SIZE];
EXTERN Tss64 tss;

EXTERN PUBLIC PROCESS proc_table[NR_TASKS];
EXTERN PROCESS* p_proc_ready;

EXTERN PUBLIC T8 task_stack[STACK_SIZE_TOTAL];

EXTERN int k_reenter;

EXTERN PUBLIC TASK task_table[NR_TASKS];
EXTERN PUBLIC TASK user_proc_table[NR_PROCS];

EXTERN PUBLIC IntHandler  irq_table[NR_IRQ];


EXTERN unsigned int ticks;

/*EXTERN PUBLIC TTY       tty_table[NR_CONSOLES];
EXTERN PUBLIC CONSOLE   console_table[NR_CONSOLES];

EXTERN int nr_current_console;
*/

#endif /* _FOS_GLOBAL_H_ */
