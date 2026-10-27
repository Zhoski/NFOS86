#ifndef PG_MEMORY_MANAGER
#define PG_MEMORY_MANAGER

#include <lib/int.h>

void pg_mk_alloc(u32 addr, u32 len);
void* pg_alloc();
void pg_free(void* pg);


#endif