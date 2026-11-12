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
#include <kernel/loader.h>
#include <kernel/process.h>
#include <lib/debug.h>

extern void isr8();
extern void isr13();
extern void isr14();
extern void isr32();
extern void isr33();
extern void isr64();
extern void isr46();

u32* krn_pg_dir;

void kmain() { 
    idt_load();                    
    idt_set(0x08, 0x08, TRAP, (uint32_t)isr8); 
    idt_set(0x0D, 0x08, TRAP, (uint32_t)isr13); 
    idt_set(0x0E, 0x08, TRAP, (uint32_t)isr14); 
    idt_set(0x20, 0x08, 0x8E, (uint32_t)isr32); 
    idt_set(0x21, 0x08, 0x8E, (uint32_t)isr33); 
    idt_set(0x2E, 0x08, 0x8E, (uint32_t)isr46);
    idt_set(0x40, 0x08, 0xEE, (uint32_t)isr64);
    pic_remap();  
    pic_irq_mask(0x21, 0b11111000); 
    pic_irq_mask(0xA1, 0b10111111); 
    init_timer(100);
    sti();
    mm_map_mk();
    //init_video();
    //pg_mk_alloc(g_mm.krn_alloc, g_mm.krn_alloc_len);
    create_heap(g_mm.krn_alloc, g_mm.krn_alloc_len);
    gdt_init();
    //pg_mapping();
    init_video();
    process_mk_tbl();
    pci_scan_bus0();               
    init_keyboard();                         

    u8* disk_info = (u8*)malloc(512);
    disk_init((u16*)disk_info);
    
    init_fs(FLUIDFS_T);

    //video->kprintf("pid0: %x\n", process_tbl[0].pid);
    //process_spawn("shell.ndf");
    //video->kprintf("pid0: %x %s %x\n", process_tbl[0].pid, process_tbl[0].pname, (u32)process_tbl[0].pg_dir);

    //u32 s = process_spawn("/apps/shell.ndf");
    //u32 s2 = process_spawn("/apps/hello.ndf");
    //process_spawn("/apps/tasm.ndf");

    //Process* pr = process_tbl;

    //video->kprintf("%s\n", pr->name);

    /*for(u32 i = 0; i < 32; i++) {
        if(process_tbl[i].pid != 0xFFFFFFFF) {
            video->kprintf("PID%d:\n name: %s\n eip: %x\n stack: %x\n",process_tbl[i].pid, process_tbl[i].name, process_tbl[i].eip, process_tbl[i].stack);
        }else {
            break;
        }
    }*/

    //video->kprintf("%s", process_tbl[0].name);

    pr_execute("/apps/shell.ndf", 0, 0);

    for(;;) {
        asm("hlt");
    }
}
