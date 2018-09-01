#define GLOBAL_VARIABLES_HERE
#include "global.h"
#include "main.h"
#include "proc.h"

TASK task_table[NR_TASKS] = {
//    {task_tty, STACK_SIZE_TTY, "tty", 150}
};

TASK user_proc_table[NR_PROCS] = {
    {testA, STACK_SIZE, "test_a", 150}
    , {testB, STACK_SIZE, "test_b", 150}
    , {testC, STACK_SIZE, "test_c", 150}
};


SysCall sys_call_table[NR_SYS_CALL] = {
    sys_get_ticks
//    , sys_write
};
