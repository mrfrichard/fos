#include "const.h"
#include "proto.h"
#include "clock.h"
#include "proc.h"
#include "global.h"
#include "string.h"
#include "mm.h"
#include "main.h"

EXTERN int disp_pos;

void testA() {
    while(1) {
        disp_color_str(BRIGHT | MAKE_COLOR(BLACK, RED), "A.");
        for (int i = 0; i < 10000; ++i) {
             for (int j = 0; j < 10000; ++j) { }
        }
//        disp_int(get_ticks());
/*        printf("<Ticks:%x>", get_ticks());
        milli_delay(200);*/
    }
}

void testB() {
    while(1) {
        disp_color_str(BRIGHT | MAKE_COLOR(BLACK, GREEN), "B.");
        for (int i = 0; i < 10000; ++i) {
             for (int j = 0; j < 10000; ++j) { }
        }

//        disp_int(get_ticks()); 
/*        printf("B");
        milli_delay(200);*/
    }
}

void testC() {
    while(1) {
        disp_color_str(BRIGHT | MAKE_COLOR(BLACK, MAZARINE), "C.");
        for (int i = 0; i < 10000; ++i) {
             for (int j = 0; j < 10000; ++j) { }
        }

/*        printf("C");
        milli_delay(200); */
    }
}



/**
* @brief ...
* 
* @return int
*/
PUBLIC int _main() {
    TASK *      p_task          = task_table;
    THREAD*     pthread         = proc_table;
    char *      p_task_stack    = (char *)task_stack + STACK_SIZE_TOTAL;

    T8 rpl;
    int eflags;

    for (int i = 0; i < NR_TASKS + NR_PROCS; i++) {
        if (i < NR_TASKS) {
            p_task = task_table + i;
            rpl = RPL_TASK;
            eflags = 0x1201; // IF = 1, IOPL = 1, bit 2 is always 1
        } else {
            p_task = user_proc_table + (i - NR_TASKS);
            rpl = RPL_USER;
            eflags = 0x202; // IF = 1, bit 2 is always 1
        }
        PROCESS* pprocess = createProcess(0, p_task->name, p_task->priority, 0);
        // strcpy(p_proc->p_name, p_task->name);
        pthread->tid = i;
        //p_proc->pml4e = 0;
        pthread->regs.cs = ((8 * INDEX_USER_C) & SA_RPL_MASK & SA_TI_MASK) | SA_TIG | rpl;
//        disp_T64(pthread->regs.cs);
        pthread->regs.fs = ((8 * INDEX_USER_RW) & SA_RPL_MASK & SA_TI_MASK) | SA_TIG | rpl;
        pthread->regs.ss = ((8 * INDEX_USER_RW) & SA_RPL_MASK & SA_TI_MASK) | SA_TIG | rpl;
//        disp_T64(pthread->regs.ss);
        pthread->regs.gs = (SELECTOR_KERNEL_GS & SA_RPL_MASK) | rpl;
        pthread->regs.rip = (T64)p_task->initial_eip;
        pthread->regs.rsp = (T64) p_task_stack;
        pthread->regs.eflags = eflags;
        pthread->ticks = pthread->priority = p_task->priority;
        pthread->pprocess = pprocess;
        //p_proc->nr_tty = 0;

        p_task_stack -= p_task->stacksize;
        pthread++;
        p_task++;
    }

    /*proc_table[1].nr_tty = 0;
    proc_table[2].nr_tty = 1;
    proc_table[3].nr_tty = 1;*/

    k_reenter = 0;
    p_proc_ready = proc_table;

    init_clock();

    restart();
    while(1){}
}
