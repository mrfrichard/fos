#include "proc.h"
#include "global.h"
#include "mm.h"

T64 pid = (T64)0;
T64 tid = (T64)0;

PUBLIC unsigned int sys_get_ticks() {
    return ticks;
}

PUBLIC void schedule() {
    int         gticks = 0;

    while (!gticks){
        LINK_NODE*  node = thread_ready_link.data;
        while (node != NULL) {
            THREAD* p = (THREAD*)(node->data);
            if (p->ticks > gticks) {
                gticks = p->ticks;
                p_proc_ready = p;
            }
            node = node->next;
        }
        
        if (!gticks) {
            node = thread_ready_link.data;
            while (node != NULL) {
                THREAD* p = (THREAD*)(node->data);
                p->ticks = p->priority;
                node = node->next;
            }
        }
    }
}