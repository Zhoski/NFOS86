#ifndef GDT_H
#define GDT_H

#include <lib/int.h>

struct gdt_entry {
    u16 limit_low;
    u16 base_low;
    u8  base_mid;
    u8  acces;
    u8  flags;
    u8  base_high;
} __attribute__((packed));

struct gdt_ptr {
    u16 limit;
    u32 base;
} __attribute__((packed));

struct tss_entry {
	u32 prev_tss; 
	u32 esp0;  
	u32 ss0;     
	u32 esp1;
	u32 ss1;
	u32 esp2;
	u32 ss2;
	u32 cr3;
	u32 eip;
	u32 eflags;
	u32 eax;
	u32 ecx;
	u32 edx;
	u32 ebx;
	u32 esp;
	u32 ebp;
	u32 esi;
	u32 edi;
	u32 es;
	u32 cs;
	u32 ss;
	u32 ds;
	u32 fs;
	u32 gs;
	u32 ldt;
	u16 trap;
	u16 iomap_base;
} __attribute__((packed));

typedef struct gdt_entry gdt_t;
typedef struct tss_entry tss_t;
typedef struct gdt_ptr gdt_ptr_t;

void gdt_init();
void gdt_add_seg(u32 base, u32 limit, u32 acess, u32 flag);

#endif