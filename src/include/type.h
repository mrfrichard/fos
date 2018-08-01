#ifndef _FOS_TYPE_H_
#define _FOS_TYPE_H_

#define PUBLIC
#define PRIVATE static
#define EXTERN extern

typedef unsigned long    T64;
typedef unsigned int     T32;
typedef unsigned short   T16;
typedef unsigned char    T8;
typedef unsigned int     TPort;
typedef char             Tbool;

typedef void (*IntHandler)();
typedef void (*TaskFun)();
typedef void (*IrqHandler)(int irq);
typedef void* SysCall;

typedef char* VaList;

#ifdef CONFIG_SMP
#define LOCK_PREFIX \
        ".section .smp_locks,\"a\"\n"    \
        "  .align 4\n"            \
"  .long 661f\n" /* address */    \
".previous\n"            \
"661:\n\tlock; "

#else /* ! CONFIG_SMP */
#define LOCK_PREFIX ""
#endif

#endif /* _FOS_TYPE_H_ */
