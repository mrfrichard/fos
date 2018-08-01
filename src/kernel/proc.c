#include "proc.h"
#include "global.h"
#include "mm.h"

T64 pid = 0;

PUBLIC PROCESS* createProcess(T64 pml4e, char* p_name, T32 priority, T32 nr_tty) {
	PROCESS* p = (PROCESS*)kmalloc(sizeof(PROCESS));
	p->pml4e = pml4e;
	p->pid = pid;
	p->p_name = p_name;
	p->priority = priority;
	p->nr_tty = nr_tty;
	p->threads = NULL;
	pid++;
	return p;
}


PUBLIC unsigned int sys_get_ticks() {
    return ticks;
}

PUBLIC void schedule() {
    THREAD*    p;
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


