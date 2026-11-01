#ifndef __MEMORY__
#define __MEMORY__
#include <lib/int.h>

void memcpy(void* __restrict__ __from,void* __restrict__ __in, u32 __c);
void memset(void* in, u8 v, u32 size);
void* malloc(u32 size);
void free(u8* ptr);
void create_heap();

#endif
