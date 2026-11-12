#ifndef PAGGING_H
#define PAGGING_H

#include <lib/int.h> 

void pg_mapping();
void pg_map_virt_phys(u32* pg_tbl, void* virtaddr, void* physaddr, u32 flag);
void* get_physaddr(u32* pg_dir, void* virtaddr);

extern u32* krn_pg_dir;

#endif
