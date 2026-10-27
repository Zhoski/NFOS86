#include <kernel/mm.h>
#include <drivers/video/video.h>

#define KERNEL_RSV      0x10000
#define KERNEL_ALLOC    0x10000
#define SATA_ACHI       0x1000

g_memory_map g_mm;

/**
 *  Парсит карту памяти полученую от вызова E820 на этапе загрузчика
 *  Находит самый большой свободный блок и режет его на зоны ядра и пользовательского софта
 *  Записывает в g_mm размер и начало каждого блока в памяти
 */
void mm_map_mk(void* mmap_addr, u16 mmapbc) {
    u32 largest_bl_len = 0;
    u32 largest_bl_addr = 0;
    for(u32 i = 0; i < mmapbc; i++) {
        u32 base_addr = *(u32*)(mmap_addr);
        u32 lenght = *(u32*)(mmap_addr + 8);
        u32 type = *(u32*)(mmap_addr + 16);
        
        if(largest_bl_len < lenght) {
            largest_bl_len = lenght;
            largest_bl_addr = base_addr;
        }

        mmap_addr += 24;
    }

    u32 mem_for_other = largest_bl_len - KERNEL_ALLOC - KERNEL_RSV - SATA_ACHI;
    g_mm.krn_addr = largest_bl_addr;
    g_mm.krn_len = KERNEL_RSV;

    g_mm.krn_alloc = largest_bl_addr + KERNEL_RSV;
    g_mm.krn_alloc_len = KERNEL_ALLOC;

    g_mm.sata_ahci = largest_bl_addr + KERNEL_RSV + KERNEL_ALLOC;
    g_mm.sata_ahci_len = SATA_ACHI;

    g_mm.other_m = largest_bl_addr + KERNEL_RSV + KERNEL_ALLOC + SATA_ACHI;
    g_mm.other_m_len = mem_for_other;
}