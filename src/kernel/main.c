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
    T8 rpl;
    int eflags;

    initThreadLink();
     for (int i = 0; i < NR_TASKS + NR_PROCS; i++) {
//    for (int i = 0; i < 1; i++) {
        if (i < NR_TASKS) {
            p_task = task_table + i;
            rpl = RPL_TASK;
            eflags = 0x1201; // IF = 1, IOPL = 1, bit 2 is always 1
        } else {
            p_task = user_proc_table + (i - NR_TASKS);
            rpl = RPL_USER;
            eflags = 0x202; // IF = 1, bit 2 is always 1
        }
        PROCESS*    pprocess   = createProcess(0, p_task->name, p_task->priority, 0);
        THREAD*     pthread    = createThread(rpl, (T64)p_task->initial_rip, p_task->stacksize, eflags, p_task->priority);
        attachThread(pprocess, pthread);
        p_task++;
    }

    /*proc_table[1].nr_tty = 0;
    proc_table[2].nr_tty = 1;
    proc_table[3].nr_tty = 1;*/

    k_reenter = 0;
    p_proc_ready = (THREAD*)(thread_ready_link.data->data);

    init_clock();

    restart();
    while(1){}
}
