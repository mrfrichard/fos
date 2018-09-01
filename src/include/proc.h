#ifndef _FOS_PROC_H_
#define _FOS_PROC_H_

#include "const.h"
#include "type.h"
#include "proto.h"
#include "protect.h"
#include "link.h"

#define STACK_SIZE    		(0x8000)
#define STACK_SIZE_TTY      (0x8000)


#define STACK_SIZE_TOTAL    (STACK_SIZE_TESTA + STACK_SIZE_TESTB + STACK_SIZE_TESTC + STACK_SIZE_TTY)

#define NR_TASKS            (0)
#define NR_PROCS            (3)

#define EBUSY               (1)

#define barrier() asm volatile("": : :"memory")
#define cpu_relax() asm volatile("pause\n": : :"memory")
#define cmpxchg(ptr,o,n) ((__typeof__(*(ptr)))__cmpxchg((ptr),(unsigned long)(o), (unsigned long)(n),sizeof(*(ptr))))

struct __xchg_dummy { unsigned long a[100]; };
#define __xg(x) ((struct __xchg_dummy *)(x))

/* Atomic exchange (of various sizes) */
static inline void *xchg_64(void *ptr, void *x) {
	__asm__ __volatile__("xchgq %0,%1"
				:"=r" ((unsigned long long) x)
				:"m" (*(volatile long long *)ptr), "0" ((unsigned long long) x)
				:"memory");

	return x;
}

static inline unsigned xchg_32(void *ptr, unsigned x){
	__asm__ __volatile__("xchgl %0,%1"
				:"=r" ((unsigned) x)
				:"m" (*(volatile unsigned *)ptr), "0" (x)
				:"memory");

	return x;
}

static inline unsigned short xchg_16(void *ptr, unsigned short x) {
	__asm__ __volatile__("xchgw %0,%1"
				:"=r" ((unsigned short) x)
				:"m" (*(volatile unsigned short *)ptr), "0" (x)
				:"memory");

	return x;
}

/* Test and set a bit */
static inline char atomic_bitsetandtest(void *ptr, int x) {
	char out;
	__asm__ __volatile__("lock; bts %2,%1\n"
						"sbb %0,%0\n"
				:"=r" (out), "=m" (*(volatile long long *)ptr)
				:"Ir" (x)
				:"memory");

	return out;
}

static inline unsigned long __cmpxchg(volatile void *ptr, unsigned long old, unsigned long new, int size) {
    unsigned long prev;
    switch (size) {
    case 1:
        __asm__ __volatile__(LOCK_PREFIX "cmpxchgb %b1,%2"
                     : "=a"(prev)
                     : "q"(new), "m"(*__xg(ptr)), "0"(old)
                     : "memory");
        return prev;
    case 2:
        __asm__ __volatile__(LOCK_PREFIX "cmpxchgw %w1,%2"
                     : "=a"(prev)
                     : "r"(new), "m"(*__xg(ptr)), "0"(old)
                     : "memory");
        return prev;
    case 4:
        __asm__ __volatile__(LOCK_PREFIX "cmpxchgl %1,%2"
                     : "=a"(prev)
                     : "r"(new), "m"(*__xg(ptr)), "0"(old)
                     : "memory");
        return prev;
	case 8:
		__asm__ __volatile__(LOCK_PREFIX "cmpxchgq %1,%2"
				   	: "=a"(prev)
					: "r"(new), "m"(*__xg(ptr)), "0"(old)
					: "memory");
		return prev;
    }
    return old;
}

typedef struct s_stackframe {
    T64 reserve;
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
    T64     pml4e;
    T64     pid;
    char*   p_name;
    T64		priority;
    T64     nr_tty;
    LINK*	threads;
} PROCESS;

typedef struct s_thread {
    STACK_FRAME regs;
    PROCESS*    pprocess;
    T64         tid;
	T64         ticks;
	T64         priority;
} THREAD;

typedef struct s_task {
    TaskFun     initial_rip;
    int         stacksize;
    char        name[PROC_NAME_LEN];
	T64         priority;
} TASK;

typedef struct _mcs_lock_t
{
	struct _mcs_lock_t *next;
	int spin;
} MCS_LOCK;

T64 pid;
T64 tid;
PUBLIC LINK thread_ready_link;
PUBLIC THREAD* p_proc_ready;

PUBLIC unsigned int sys_get_ticks();
PUBLIC void schedule();

static inline void initThreadLink() {
	createLink(&thread_ready_link);
}

static inline void lock_mcs(MCS_LOCK** m, MCS_LOCK* me)
{
//	disp_str("lock_mcs");
//	disp_T64(m);
//    disp_T64(*m);
//	disp_T64(me);
	MCS_LOCK* tail;

	me->next = NULL;
	me->spin = 0;

	tail = xchg_64(m, me);

	/* No one there? */
	if (!tail) return;

	/* Someone there, need to link in */
	tail->next = me;

	/* Make sure we do the above setting of next. */
	barrier();

	/* Spin on my spin variable */
	while (!me->spin) cpu_relax();

	return;
}

static inline void unlock_mcs(MCS_LOCK** m, MCS_LOCK* me)
{
//	disp_str("unlock_mcs");
//	disp_T64(m);
//    disp_T64(*m);
//	disp_T64(me);
	/* No successor yet? */
	if (!me->next)
	{
//        disp_str("if ");
		/* Try to atomically unlock */
		if (cmpxchg(m, me, NULL) == me) {
		    return;
		}
//        disp_str("while ");
		/* Wait for successor to appear */
		while (!me->next) cpu_relax();
	}

	/* Unlock next one */
	me->next->spin = 1;
}

static inline int trylock_mcs(MCS_LOCK** m, MCS_LOCK* me)
{
	MCS_LOCK *tail;

	me->next = NULL;
	me->spin = 0;

	/* Try to lock */
	tail = cmpxchg(m, NULL, &me);

	/* No one was there - can quickly return */
	if (!tail) return 0;

	return EBUSY;
}

static inline THREAD* _createThread(T64 cs, T64 fs, T64 ss, T64 gs, T64 rip, T64 rsp, T64 flags, T64 priority) {
	THREAD* pthread = (THREAD*)kmallocAlign16(sizeof(THREAD));
	if (pthread != NULL) {
        pthread->tid = tid;
        pthread->regs.cs = cs;
        pthread->regs.fs = fs;
        pthread->regs.ss = ss;
        pthread->regs.gs = gs;
        pthread->regs.rip = rip;
        pthread->regs.rsp = rsp;
        pthread->regs.eflags = flags;
        pthread->ticks = pthread->priority = priority;
        tid ++;
	} else {
        disp_color_str(RED, "create thread failed/n");
	}

	return pthread;
}

static inline THREAD* createThread(T8 rpl, T64 rip, T64 stackSize, T64 flags, T64 priority) {
	void* rsp = kmalloc(STACK_SIZE);
	if (rsp != NULL) {
		(*(long*)(void*)&rsp) += stackSize;
		return _createThread(
				((8 * INDEX_USER_C) & SA_RPL_MASK & SA_TI_MASK) | SA_TIG | rpl
				, ((8 * INDEX_USER_RW) & SA_RPL_MASK & SA_TI_MASK) | SA_TIG | rpl
				, ((8 * INDEX_USER_RW) & SA_RPL_MASK & SA_TI_MASK) | SA_TIG | rpl
				, (SELECTOR_KERNEL_GS & SA_RPL_MASK) | rpl
				, rip, (T64)rsp, flags, priority
		);
	}
    disp_color_str(RED, "alloc stack failed/n");
	return NULL;
}

static inline PUBLIC PROCESS* createProcess(T64 pml4e, char* p_name, T64 priority, T64 nr_tty) {
	PROCESS* p = (PROCESS*)kmalloc(sizeof(PROCESS));
	if (p != NULL) {
        p->pml4e = pml4e;
        p->pid = pid;
        p->p_name = p_name;
        p->priority = priority;
        p->nr_tty = nr_tty;
        p->threads = NULL;
        pid++;
	} else {
        disp_color_str(RED, "create process fill/n");
	}
	return p;
}

static inline BOOL attachThread(PROCESS* process, THREAD* thread) {
	if (
			(process != NULL)
			&& (thread != NULL)
			&& linkCreateAppend_data(&(process->threads), (void*)thread)
			&& linkAppend_date(&thread_ready_link, (void*)thread)
		) {
		thread->pprocess = process;
		return TRUE;
	}
    disp_color_str(RED, "attach thread fill/n");
	return FALSE;
}
#endif /* _FOS_PROC_H_ */
