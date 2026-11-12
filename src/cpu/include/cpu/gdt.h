#ifndef GDT_H
#define GDT_H

#include <lib/int.h>

struct descriptor {
    u16 limit_low;
    u16 base_low;
    u8  base_mid;
    u8  acces;
    u8  limit_hight:4;
    u8  flag:4;
    u8  base_high;
} __attribute__((packed));

struct desc_ptr {
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

typedef struct descriptor desc_t;
typedef struct tss_entry tss_t;
typedef struct desc_ptr desc_ptr_t;

void gdt_init();
u32  gdt_descriptor_add(u32 base, u32 limit, u32 acess, u32 flag);
u32 ldt_mk_and_push(u32 base, u32 limit);

extern void* gdt_addr;

#endif
