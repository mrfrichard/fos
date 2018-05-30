#include "proc.h"
#include "global.h"


PUBLIC unsigned int sys_get_ticks() {
    return ticks;
}

PUBLIC void schedule() {
    PROCESS*    p;
    int         gticks = 0;

    while (!gticks){
        for (p = proc_table; p < proc_table + NR_TASKS + NR_PROCS; p++) {
            if (p->ticks > gticks) {
                gticks = p->ticks;
                p_proc_ready = p;
            }
        }
        
        if (!gticks) {
            for (p = proc_table; p < proc_table + NR_TASKS + NR_PROCS; p++) {
                p->ticks = p->priority;
            }
        }
    }
}


