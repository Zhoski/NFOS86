#include <cpu/gdt.h>
#include <kernel/pmm.h>
#include <drivers/video/video.h>
#include <kernel/memory.h>

void* gdt_addr;
u32 gdt_offset = 8;

gdt_ptr_t ptr;

extern void gdt_reload_addr(void* gdt_addr);

void gdt_add_seg(u32 base, u32 limit, u32 acess, u32 flag) {
    gdt_t descriptor = {
        .base_low = base,
        .base_mid = ((base >> 16) & 0xFF),
        .base_high = ((base >> 24) & 0xFF),
        .limit_low = limit,
        .acces = acess,
        .flags = (limit >> 16) << 4 | flag,
    };

    memcpy(&descriptor, gdt_addr + gdt_offset, sizeof(gdt_t));
    gdt_offset += 8;
}

void ltr(u16 selector) {
    asm volatile(
        "movw %0, %%ax\n"
        "ltr %%ax"
        :
        : "r" ((u16)((selector << 3) | 0))
        : "eax"
    );
}

void gdt_init() {
    gdt_addr = pg_alloc();

    ptr.limit = 0xFFF;
    ptr.base = (u32)gdt_addr;

    tss_t tss0;
    tss0.esp0 = (u32)(pg_alloc() + 0xFF0);
    tss0.ss0 = 0x10;

    gdt_add_seg(0, 0xFFFFF, 0x9A, 0x0C);                // Kernel code segment
    gdt_add_seg(0, 0xFFFFF, 0x92, 0x0C);                // Kernel data segment
    gdt_add_seg(0, 0xFFFFF, 0xFA, 0x0C);                // User code segment
    gdt_add_seg(0, 0xFFFFF, 0xF2, 0x0C);                // User data segment
    gdt_add_seg((u32)&tss0, sizeof(tss_t) - 1, 0x89, 0x00);  // TSS

    gdt_reload_addr(&ptr);
    ltr(5);

    //gdt_reload(gdt_addr);
}