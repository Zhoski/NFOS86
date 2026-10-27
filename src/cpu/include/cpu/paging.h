#ifndef PAGGING_H
#define PAGGING_H

#include <lib/int.h> 


void pg_mapping();
void pg_map(void* physaddr, void* virtaddr, u32 flag);
void* get_physaddr(void* virtaddr);

#endif