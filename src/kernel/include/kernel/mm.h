#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <lib/int.h>

// Карта содержащая размер и начало каждого блока в памяти
typedef struct {
    u32 krn_addr;
    u32 krn_len;
    u32 krn_alloc;
    u32 krn_alloc_len;
    u32 sata_ahci;
    u32 sata_ahci_len;
    u32 other_m;
    u32 other_m_len;
} g_memory_map;

void mm_map_mk(void* mmap, u16 mmapbc);

extern g_memory_map g_mm;

#endif