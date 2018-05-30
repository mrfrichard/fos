#ifndef _FOS_TYPE_H_
#define _FOS_TYPE_H_

#define PUBLIC
#define PRIVATE static
#define EXTERN extern

typedef unsigned long   T64;
typedef unsigned int     T32;
typedef unsigned short   T16;
typedef unsigned char    T8;
typedef unsigned int     TPort;
typedef char            Tbool;

typedef void (*IntHandler)();
typedef void (*TaskFun)();
typedef void (*IrqHandler)(int irq);
typedef void* SysCall;

typedef char* VaList;

#endif /* _FOS_TYPE_H_ */
