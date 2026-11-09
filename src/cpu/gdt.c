#include <cpu/gdt.h>
#include <kernel/pmm.h>
#include <drivers/video/video.h>
#include <kernel/memory.h>

void* gdt_addr;
void* ldt_addr;
u32 gdt_offset = 8;

desc_ptr_t ptr;
tss_t tss0;

extern void gdt_reload_addr(void* gdt_addr);

void gdt_add_seg(u32 base, u32 limit, u32 acess, u32 flag) {
    desc_t descriptor = {
        .base_low = base,
        .base_mid = ((base >> 16) & 0xFF),
        .base_high = ((base >> 24) & 0xFF),
        .limit_low = limit,
        .acces = acess,
        .flag  = flag & 0x0F,
        .limit_hight = (limit >> 16) & 0x0F,
    };

    memcpy(&descriptor, gdt_addr + gdt_offset, sizeof(desc_t));
    gdt_offset += 8;
}

void mk_descriptor(desc_t* desc_t, u32 base, u32 limit, u32 acess, u32 flag) {
    desc_t->base_low = base;
    desc_t->base_mid = ((base >> 16) & 0xFF);
    desc_t->base_high = ((base >> 24) & 0xFF);
    desc_t->limit_low = limit & 0xFFFF;
    desc_t->acces = acess;
    desc_t->flag = flag & 0x0F; 
    desc_t->limit_hight = (limit >> 16) & 0x0F;

    //video->kprintf("limit: %x\nflag: %x", desc_t->limit_hight << 16 | desc_t->limit_low, desc_t->flag);

    //for(;;) asm("hlt");
}

void ltr(u16 selector) {
    asm volatile(
        "movw %0, %%ax\n"
        "ltr %%ax"
        :
        : "r" ((u16)((selector << 3)))
        : "eax"
    );
}

void lldt(u16 selector) {
    asm volatile(
        "movw %0, %%ax\n"
        "lldt %%ax"
        :
        : "r" ((u16)((selector << 3)))
        : "eax"
    );
}

void ldt_mk_and_push(u32 base, u32 limit) {
    void* ldt = malloc(16);

    desc_t* ds = (desc_t*)ldt;
    mk_descriptor(ds, base, limit, 0xFA, 0x0C);
    mk_descriptor((desc_t*)(ds + 1), base, limit, 0xF3, 0x0C);
    
    gdt_add_seg((u32)ldt, 15, 0x82, 0x00);

    lldt(6);
}


void gdt_init() {
    gdt_addr = pg_alloc();

    ptr.limit = 0xFFF;
    ptr.base = (u32)gdt_addr;

    tss0.esp0 = (u32)(pg_alloc() + 0xFFF);
    tss0.ss0 = 0x10;

    gdt_add_seg(0, 0xFFFFF, 0x9A, 0x0C);                        // Kernel code segment
    gdt_add_seg(0, 0xFFFFF, 0x92, 0x0C);                        // Kernel data segment
    gdt_add_seg(0x200000, 0xFFFFF, 0xFA, 0x0C);                 // User code segment
    gdt_add_seg(0x200000, 0xFFFFF, 0xF3, 0x0C);                 // User data segment
    gdt_add_seg((u32)&tss0, sizeof(tss_t) - 1, 0x89, 0x00);     // TSS

    gdt_reload_addr(&ptr);
    ltr(5);
}
