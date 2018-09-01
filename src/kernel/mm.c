#include "mm.h"
#include "proc.h"

MCS_LOCK* p_mm_lock = NULL;

PUBLIC void* kmalloc(int size) {
    MEM_BLOCK* pblock =  (MEM_BLOCK*)KERNEL_DYNAMIC_MEM_BASE;
    void* p = NULL;
    int _size = size + sizeof(T64);

    MCS_LOCK me;
    lock_mcs(&p_mm_lock, &me);

    int limit = (_size / sizeof(void*) + 1) * sizeof(void*);

    while (pblock->next != NULL) {
        if (pblock->next->len >= _size) {
            T64* plen = (T64 *)(pblock->next);
            if (pblock->next->len <= limit) {   // 按照指针长度对齐分配内存，减小内存碎片数量
                pblock->next = pblock->next->next;
            } else {
                if (_size % sizeof(void *)) {
                    _size = limit;
                }
                MEM_BLOCK* pnext = (MEM_BLOCK *)((void *)(pblock->next) + _size);
                pnext->next = pblock->next->next;
                pnext->len = pblock->next->len - _size;
                pblock->next = pnext;
            }
            *plen = _size;
            plen++;
            p = (void*) plen;
            break;
        }
        pblock = pblock->next;
    }

    unlock_mcs(&p_mm_lock, &me);
    return p;
}


PUBLIC void* kmallocAlign16(int size) {
    MEM_BLOCK* pblock =  (MEM_BLOCK*)KERNEL_DYNAMIC_MEM_BASE;
    void* p = NULL;
    int _size = size + sizeof(T64);
    int _size2 = _size + sizeof(T64);

    MCS_LOCK me;
    lock_mcs(&p_mm_lock, &me);

    int limit = (_size / sizeof(void*) + 1) * sizeof(void*);
    int limit2 = limit + sizeof(T64);

    while (pblock->next != NULL) {
        if (pblock->next->len >= _size) {
            T64* plen = (T64 *)(pblock->next);
            unsigned long _rev = (unsigned long)plen & 0xf;
            if (_rev == sizeof(T64)) {
                if (pblock->next->len <= limit) {   // 按照指针长度对齐分配内存，减小内存碎片数量
                    pblock->next = pblock->next->next;
                } else {
                    if (_size % sizeof(void *)) {
                        _size = limit;
                    }
                    MEM_BLOCK* pnext = (MEM_BLOCK *)((void *)(pblock->next) + _size);
                    pnext->next = pblock->next->next;
                    pnext->len = pblock->next->len - _size;
                    pblock->next = pnext;
                }
                *plen = _size;
                plen++;
                p = (void*) plen;
                break;
            } else if (pblock->next->len >= _size2){
                if (pblock->next->len <= limit2) {   // 按照指针长度对齐分配内存，减小内存碎片数量
                    pblock->next = pblock->next->next;
                } else {
                    if (_size2 % sizeof(void *)) {
                        _size2 = limit2;
                    }
                    MEM_BLOCK* pnext = (MEM_BLOCK *)((void *)(pblock->next) + _size2);
                    pnext->next = pblock->next->next;
                    pnext->len = pblock->next->len - _size2;
                    pblock->next = pnext;
                }
                *plen = _size2;
                plen++;
                *plen = REV_TAG;
                plen++;
                p = (void*) plen;
                break;
            }
        }
        pblock = pblock->next;
    }

    unlock_mcs(&p_mm_lock, &me);
    return p;
}

PUBLIC void kfree(void* p) {
    void* pstart = p;
    pstart -= sizeof(T64);
    T64 len = *((T64*)pstart);
    if (len == REV_TAG) {
        pstart -= sizeof(T64);
        len = *((T64*)pstart);
    }
    void* pend = pstart + len;

    MEM_BLOCK* pblock =  (MEM_BLOCK*)KERNEL_DYNAMIC_MEM_BASE;
    MEM_BLOCK* pb = (MEM_BLOCK*)pstart;

    MCS_LOCK me;
    lock_mcs(&p_mm_lock, &me);

    while (pblock->next != NULL) {
        if ((unsigned long)pend < (unsigned long)(pblock->next)) {
            pb->next = pblock->next;
            pblock->next = pb;
        } else if ((unsigned long)pend == (unsigned long)(pblock->next)) {
            pb->next = pblock->next->next;
            pb->len += pblock->next->len;
            pblock->next = pb;
        } else {
            void* pblockend = (void *)(pblock->next) + pblock->next->len;

            if (pstart < pblockend) {
                // TODO: show error
            } else if (pstart == pblockend) {
                pblock->next->len += pb->len;
            } else {
                if (pblock->next->next != NULL) {
                    pblock = pblock->next;
                } else {
                    pb->next = NULL;
                    pblock->next->next = pb;
                }
            }
        }
    }

    unlock_mcs(&p_mm_lock, &me);
}
