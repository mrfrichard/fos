#ifndef _TINIX_STRING_H_
#define _TINIX_STRING_H_
#include "type.h"
PUBLIC void memcpy(void* pDst, void* pSrc, int iSize);
PUBLIC void memset(void* pDst, T8 value, int iSize);
PUBLIC int strcpy(char* pDst, char* pSrc);

#endif /* _TINIX_STRING_H_ */
