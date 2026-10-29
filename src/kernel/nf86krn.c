#include <drivers/video/video.h>
#include <drivers/timer/timer.h>
#include <drivers/keyboard/keyboard.h>
#include <drivers/disk/disk.h>
#include <drivers/pci/pci.h>
#include <cpu/cpu.h>
#include <cpu/idt.h>
#include <cpu/pic.h>
#include <cpu/gdt.h>
#include <cpu/io.h>
#include <fs/fs.h>
#include <kernel/syscall.h>
#include <kernel/memory.h>
#include <cpu/paging.h>
#include <kernel/mm.h>
#include <kernel/pmm.h>

extern void isr8();
extern void isr13();
extern void isr14();
extern void isr32();
extern void isr33();
extern void isr64();
extern void isr46();

void kmain() {
    idt_load();                    
    idt_set(0x08, 0x08, 0x8E, (uint32_t)isr8); 
    idt_set(0x08, 0x08, 0x8E, (uint32_t)isr13); 
    idt_set(0x08, 0x08, 0x8E, (uint32_t)isr14); 
    idt_set(0x20, 0x08, 0x8E, (uint32_t)isr32); 
    idt_set(0x21, 0x08, 0x8E, (uint32_t)isr33); 
    idt_set(0x2E, 0x08, 0x8E, (uint32_t)isr46);
    idt_set(0x40, 0x08, 0xEE, (uint32_t)isr64);
    pic_remap();  
    pic_irq_mask(0x21, 0b11111000); 
    pic_irq_mask(0xA1, 0b10111111); 
    sti();
    pci_scan_bus0();
    init_memory();                 
    create_heap();
    init_keyboard();                         
    init_vga(VGA_640_480);
    mm_map_mk();
    pg_mk_alloc(g_mm.krn_alloc, g_mm.krn_alloc_len);
    gdt_init();
    pg_mapping();

    u8* disk_info = (u8*)malloc(512);
    disk_init((u16*)disk_info);

    init_fs(FLUIDFS_T);    

    init_timer(100);

    fs->open("/home/shell.ndf");
}
