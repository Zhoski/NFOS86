#include <kernel/loader.h>
#include <drivers/video/video.h>
#include <kernel/memory.h>
#include  <cpu/gdt.h>
#include <fs/fs.h>
#include <lib/string.h>
#include <kernel/process.h>
#include <lib/align.h>

uint32_t kernel_stack_ptr;
uint32_t kernel_stack_base;
uint32_t kernel_return_ptr;

u32 debug = 0;

typedef void (*entry_point)();

void _exit();
extern void jmp_usrmode(u32 entry, u32 stack);

u32 virt_entry_point;

u32 g_argc;
u8** g_argv;

u32 ndf_load(u8* f,void* phys_entry, u32 virt_entry) {
    u32 st = fs->read(f, phys_entry, g_node.size);
    if(st != 0) return 0;

    ndf_header_t ndf_header;
    memcpy(phys_entry, &ndf_header, sizeof(ndf_header));
    
    u32 phys_addr = (u32)phys_entry;

    memcpy((void*)(phys_addr + sizeof(ndf_header_t)), (void*)(phys_addr), ndf_header.fsize - sizeof(ndf_header_t));
    
    u32* patch_offset = (u32*)(phys_addr + ndf_header.reloff - sizeof(ndf_header_t));
    for(u32 k = 0; k < ndf_header.relsz / 4; k++) {
        u32* addr_patch = (u32*)(phys_addr + patch_offset[k] - sizeof(ndf_header_t));
        *addr_patch = *addr_patch + virt_entry;
    }

    virt_entry_point = (u32)(virt_entry + ndf_header.entry - sizeof(ndf_header_t) + 28);
}

u32 execute(u8* f, u32 argc, u8* argv[]) {
    Process* process = process_spawn(f);
    if(!process) return 0;

    void* phys_entry = (void*)process->base;
    void* virt_stack = (void*)process->stack_limit;

    virt_stack = (void*)align((u32)virt_stack, 16, LOWER);

    void* load_entry = phys_entry + process->stack_limit + 1;

    ndf_load(f, load_entry, align(process->stack_limit + 1, 1024, 1));

    jmp_usrmode(virt_entry_point, (u32)virt_stack);
    
    /*video->kprintf("Execute\n");
    g_argc = argc;
    g_argv = argv;

    u32 pid = process_spawn(f);
    if(pid == 0xFFFFFFFF) {
        return 0;
    }

    Process* process = process_tbl + pid;

    void* load_entry = process->eip;//(void*)0x200000;
    void* stack = process->stack;//(void*)0x210000;

    video->kprintf("Base %x\nStack: %x\nHeap: %x\n", load_entry, stack, (u32)process->heap_border);
    //for(;;);
 
    fs->read(f, load_entry, g_node.size);

    if(*(u8*)load_entry == 0x77) patch((void*)load_entry, 0x000000);
    
    kernel_return_ptr = (uint32_t)&_exit;
    asm volatile("movl %%esp, %0":"=r" (kernel_stack_ptr));
    asm volatile("movl %%ebp, %0":"=r" (kernel_stack_base));

    u32 offset = 0;

    for(u32 i = 0; i < argc; i++) {
        u32 arg_len = strlen(argv[i]) + 1;
        offset += arg_len;
        memcpy(argv[i], stack - offset, arg_len);
        g_argv[i] = stack - offset;
        offset = align(offset, 4);
    }

    for(u32 i = argc; i > 0; i--) {
        offset += 4;
        u32* push_add = (u32*)(stack - offset);
        *push_add = (u32)g_argv[i - 1];
    }

    g_argv = (u8**)(stack - offset);

    offset += 4;
    u32* push_add = (u32*)(stack - offset);
    *push_add = (u32)g_argv;
    offset += 4;
    push_add = (u32*)(stack - offset);
    *push_add = argc;

    //ldt_mk_and_push((u32)load_entry, 15);

    //for(;;) asm("hlt");

    jmp_usrmode(program, (u32)stack - offset - 4 - (u32)load_entry);*/
}

void _exit() {
    execute("/apps/shell.ndf",0,0);

    for(;;) {
        asm("hlt");
    }
}
