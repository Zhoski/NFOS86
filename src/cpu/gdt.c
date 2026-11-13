#include <cpu/gdt.h>
#include <kernel/pmm.h>
#include <drivers/video/video.h>
#include <kernel/memory.h>

#define MAX_DESC 512

void* gdt_addr;
void* ldt_addr;
u32 gdt_offset = 8;

u8 gdt_bitmap[64];

desc_ptr_t ptr;
tss_t tss0;

extern void gdt_reload_addr(void* gdt_addr);

u32 gdt_descriptor_add(u32 base, u32 limit, u32 acess, u32 flag) {
    desc_t descriptor = {
        .base_low = base,
        .base_mid = ((base >> 16) & 0xFF),
        .base_high = ((base >> 24) & 0xFF),
        .limit_low = limit,
        .acces = acess,
        .flag  = flag & 0x0F,
        .limit_hight = (limit >> 16) & 0x0F,
    };

    for(u32 i = 0; i < MAX_DESC; i++) {
        u32 byte = i / 8;
        u32 bit_in_byte = i % 8;
        if((gdt_bitmap[byte] & (1 << bit_in_byte)) == 0) {
            gdt_bitmap[byte] |= (1 << bit_in_byte);
            memcpy(&descriptor, gdt_addr + (i << 3), sizeof(desc_t));
            return i;
        }
    }

    return 0;
    
    //memcpy(&descriptor, gdt_addr + (bit << 3), sizeof(desc_t));
}

void gdt_descriptor_rm(u32 sel) {
    u32 byte = sel / 8;
    u32 bit = sel % 8;

    gdt_bitmap[byte] &= ~bit;
}

void mk_descriptor(desc_t* desc_t, u32 base, u32 limit, u32 acess, u32 flag) {
    desc_t->base_low = base;
    desc_t->base_mid = ((base >> 16) & 0xFF);
    desc_t->base_high = ((base >> 24) & 0xFF);
    desc_t->limit_low = limit & 0xFFFF;
    desc_t->acces = acess;
    desc_t->flag = flag & 0x0F; 
    desc_t->limit_hight = (limit >> 16) & 0x0F;
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

u32 ldt_mk_and_push(u32 base, u32 limit, u32 stack_limit) {
    void* ldt = malloc(24);

    desc_t* ds = (desc_t*)ldt;
    mk_descriptor(ds, base, limit, 0xFA, 0x04);
    mk_descriptor((desc_t*)(ds + 1), base, limit, 0xF3, 0x04);
    mk_descriptor((desc_t*)(ds + 2), base, stack_limit, 0xF3, 0x04);
    
    u32 sel = gdt_descriptor_add((u32)ldt, 23, 0x82, 0x00);

    lldt(sel);

    return sel << 3;
}

void gdt_init() {
    for(u32 i = 0; i < 64; i++) {
        gdt_bitmap[i] = 0;
    }
    gdt_bitmap[0] |= 1;
    gdt_addr = (void*)0x1A3000;

    ptr.limit = 0xFFF;
    ptr.base = (u32)gdt_addr;

    tss0.esp0 = (u32)(0x1A4000 + 0xFFF);
    tss0.ss0 = 0x10;

    gdt_descriptor_add(0, 0xFFFFF, 0x9A, 0x0C);                        // Kernel code segment
    gdt_descriptor_add(0, 0xFFFFF, 0x92, 0x0C);                        // Kernel data segment
    gdt_descriptor_add(0, 0xFFFFF, 0xFA, 0x0C);                        // User code segment
    gdt_descriptor_add(0, 0xFFFFF, 0xF3, 0x0C);                        // User data segment
    gdt_descriptor_add((u32)&tss0, sizeof(tss_t) - 1, 0x89, 0x00);     // TSS

    gdt_reload_addr(&ptr);
    ltr(5);
}
