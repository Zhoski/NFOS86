#include <cpu/paging.h>
#include <drivers/video/video.h>
#include <kernel/memory.h>
#include <kernel/pmm.h>

u32 *krn_page_dir;
u32 *krn_page_tbl0;

u32 *pc0_page_dir;
u32 *pc0_page_tbl0;

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

void pg_mapping() {
    krn_page_dir = pg_alloc();
    krn_page_tbl0 = pg_alloc();

    // Таблицы для процесса
    pc0_page_dir = pg_alloc();     
    pc0_page_tbl0 = pg_alloc();

    for(u32 k = 0; k < 1024; k++) {
        krn_page_dir[k] = 0x00;
    }

    for(u32 k = 0; k < 512; k++) {
        krn_page_tbl0[k] = (k << 12) | 3;
    }

    for(u32 k = 512; k < 1024; k++) {
        krn_page_tbl0[k] = (k << 12) | 7;
    }

    krn_page_tbl0[1] = 0x1000 | 7;      // Разрешить всем читать эту страницу, но не изменять

    krn_page_dir[0] = ((u32)krn_page_tbl0) | 7;


    pdbr_load((u32)krn_page_dir);

    pg_enable();
}

void* get_physaddr(void* virtaddr) {
    u32 dir = (u32)virtaddr >> 22;
    u32 page = (u32)virtaddr >> 12 & 0x3FF;
    u32 offset = (u32)virtaddr & 0xFFF;

    u32* physaddr = (u32*)(krn_page_dir[dir] & ~0xFFF);

    //video->kprintf("%x", *(u32*)(physaddr + page) & ~0xFFF + offset);

    return (void*)(*(u32*)(physaddr + page) & ~0xFFF + offset);
}

/**
 *  Мапит физический адрес к виртуальному
 */
void pg_map(void* physaddr, void* virtaddr, u32 flag) {
    u32 dir = (u32)physaddr >> 22;
    u32 page = (u32)physaddr >> 12 & 0x3FF;

    if(krn_page_dir[dir] == 0x00) {
        u32* pg_tbl = pg_alloc();
        pg_tbl[page] = (u32)physaddr | flag;

        krn_page_dir[dir] = ((u32)pg_tbl) | flag;
        //video->kprintf("pg dir: %x\n", krn_page_dir[dir]);
        //video->kprintf("pg tbl: %x\n", pg_tbl[page]);
    }

    u32* ptr = (u32*)(krn_page_dir[dir] & ~0xFFF);

    //video->kprintf("%x", *(ptr + page));

    *(ptr + page) = (u32)physaddr | flag;

    //video->kprintf("Dir: %d     Page: %d\n", dir, page);

    /*u32* test = krn_page_dir[dir] & ~0xFFF;
    video->kprintf("Virtaddr: %x\n", *(test + page));*/

    /*for(;;) {
        asm("hlt");
    }*/
}
