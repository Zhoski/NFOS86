#include <cpu/paging.h>
#include <drivers/video/video.h>
#include <kernel/memory.h>
#include <kernel/pmm.h>

u32 *page_dir;
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

void pg_mapping() {
    page_dir = pg_alloc();
    page_tbl0 = pg_alloc();

    for(u32 k = 0; k < 1024; k++) {
        page_dir[k] = 0x00;
    }

    for(u32 k = 0; k < 1024; k++) {
        page_tbl0[k] = (k * 0x1000) | 7;
    }

    page_dir[0] = ((u32)page_tbl0) | 7;

    pdbr_load(page_dir);

    pg_enable();
}

void* get_physaddr(void* virtaddr) {
    u32 dir = (u32)virtaddr >> 22;
    u32 page = (u32)virtaddr >> 12 & 0x3FF;
    u32 offset = (u32)virtaddr & 0xFFF;

    u32* physaddr = page_dir[dir] & ~0xFFF;

    //video->kprintf("%x", *(u32*)(physaddr + page) & ~0xFFF + offset);

    return (void*)(*(u32*)(physaddr + page) & ~0xFFF + offset);
}

/**
 *  Мапит физический адрес к виртуальному
 */
void pg_map(void* physaddr, void* virtaddr, u32 flag) {
    u32 dir = (u32)physaddr >> 22;
    u32 page = (u32)physaddr >> 12 & 0x3FF;

    if(page_dir[dir] == 0x00) {
        u32* pg_tbl = pg_alloc();
        pg_tbl[page] = (u32)physaddr | flag;

        page_dir[dir] = ((u32)pg_tbl) | flag;
        //video->kprintf("pg dir: %x\n", page_dir[dir]);
        //video->kprintf("pg tbl: %x\n", pg_tbl[page]);
    }

    u32* ptr = page_dir[dir] & ~0xFFF;

    //video->kprintf("%x", *(ptr + page));

    *(ptr + page) = (u32)physaddr | flag;

    //video->kprintf("Dir: %d     Page: %d\n", dir, page);

    /*u32* test = page_dir[dir] & ~0xFFF;
    video->kprintf("Virtaddr: %x\n", *(test + page));*/

    /*for(;;) {
        asm("hlt");
    }*/
}