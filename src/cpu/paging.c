#include <cpu/paging.h>
#include <drivers/video/video.h>
#include <kernel/memory.h>
#include <kernel/pmm.h>

#define KRN_PAGE_RW_P    0x03
#define KRN_PAGE_RW      0x02
#define USR_PAGE_RW_P    0x07
#define USR_PAGE_RW      0x06

u32 *krn_pg__dir;
u32 *page_tbl0;

void pg_enable() {
    asm(
        "movl %%cr0, %%eax\n"
        "orl $0x80000000, %%eax\n"
        "movl %%eax, %%cr0\n"
        :
        :
        : "eax", "cc"
    );
}

void pdbr_load(u32 pdbr) {
    asm(
        "movl %0, %%cr3\n"
        :
        : "r" (pdbr)
        : "eax", "cc"
    );
}

/**
 *  Мапит память нижние 2мб под ядро, верхние под программы
 * 
 *  Ядро не видит память выше 4мб
 */
void pg_mapping() {
    krn_pg_dir = (u32*)0x1A5000;
    page_tbl0 = (u32*)0x1A6000;
    for(u32 k = 0; k < 1024; k++) {
        krn_pg_dir[k] = 0x00;
    }
    for(u32 k = 0; k < 512; k++) {
        page_tbl0[k] = (k * 0x1000) | KRN_PAGE_RW_P;
    }
    for(u32 k = 512; k < 1024; k++) {
        page_tbl0[k] = (k << 12) | USR_PAGE_RW_P;
    }
    page_tbl0[1] = 0x1000 | 0x07;   // Разрешить читать всем
    krn_pg_dir[0] = ((u32)page_tbl0) | USR_PAGE_RW_P;
    pdbr_load((u32)krn_pg_dir);
    //pg_enable();
}

void* get_physaddr(u32* pg_dir, void* virtaddr) {
    u32 dir = (u32)virtaddr >> 22;
    u32 page = (u32)virtaddr >> 12 & 0x3FF;
    u32 offset = (u32)virtaddr & 0xFFF;

    u32* physaddr = (u32*)(pg_dir[dir] & ~0xFFF);

    return (void*)(*(u32*)(physaddr + page) & ~0xFFF | offset);
}

/**
 *  Мапит виртуальный адрес к физическому
 */
void pg_map_virt_phys(u32* pg_dir, void* virtaddr, void* physaddr, u32 flag) {
    u32 dir = (u32)virtaddr >> 22;
    u32 page = (u32)virtaddr >> 12 & 0x3FF;

    if(pg_dir[dir] == 0x00) {
        u32* pg_tbl = pg_alloc(1);
        pg_tbl[page] = (u32)virtaddr | flag;

        pg_dir[dir] = ((u32)pg_tbl) | flag;
    }

    u32* ptr = (u32*)(pg_dir[dir] & ~0xFFF);
    *(ptr + page) = (u32)virtaddr | flag;
}
