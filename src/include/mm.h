#ifndef _FOS_MM_H_
#define _FOS_MM_H_

#include "type.h"

#define DIRECT_MAPPING_PHYS_BASE (0xffff880000010000)

#define _KERNEL_DYNAMIC_MEM_BASE (0x80000)

#define KERNEL_DYNAMIC_MEM_BASE (DIRECT_MAPPING_PHYS_BASE + _KERNEL_DYNAMIC_MEM_BASE)
#define KERNEL_DYNAMIC_MEM_LEN (0x80000)

typedef struct _mem_block {
    int len;
    struct _mem_block* next;
} MEM_BLOCK;

PUBLIC void     initKernelDynamicMem();
PUBLIC void*    kmalloc(int size);
PUBLIC void     kfree(void* p);

#endif /* _FOS_I8259_H_ */
