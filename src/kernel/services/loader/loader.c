#include <kernel/loader.h>
#include <drivers/video/video.h>
#include <kernel/program.h>
#include <kernel/memory.h>
#include <fs/fs.h>

typedef void (*entry_point)();

void _exit();
extern void jmp_usrmode(entry_point entry);

volatile u32 debug = 0;

void execute_file(void* entry, void* stack, ext_t ext) {
    video->kprintf("Execute file\n");
    ndf_header_t ndf_header;
    memcpy(entry, &ndf_header, sizeof(ndf_header));
    
    u32 entry_addr = (u32)entry;

    memcpy((void*)(entry_addr + sizeof(ndf_header_t)), (void*)(entry_addr), ndf_header.fsize - sizeof(ndf_header_t));

    u32* patch_offset = (u32*)(entry_addr + ndf_header.reloff - sizeof(ndf_header_t));
    for(u32 k = 0; k < ndf_header.relsz / 4; k++) {
        u32* addr_patch = (u32*)(entry_addr + patch_offset[k] - sizeof(ndf_header_t));
        if(debug) {
            video->kprintf("[ INFO ] Patching addr: %x  Offset: %x  Data: %x\n", addr_patch, patch_offset[k], *addr_patch);
        }
        *addr_patch = *addr_patch + entry_addr;
    }

    //video->kprintf("Stop point\n");
    //video->kprintf("krn: %x\n", kernel_return_ptr);

    kernel_return_ptr = (uint32_t)&_exit;
   
    // +28 потому что это размер призрачной секции которая почему то остается в ndf
    volatile entry_point program = (entry_point)(entry_addr + ndf_header.entry - sizeof(ndf_header_t) + 28);

    //video->kprintf("%x\n", program);

    asm volatile("movl %%esp, %0":"=r" (kernel_stack_ptr));
    asm volatile("movl %%ebp, %0":"=r" (kernel_stack_base));

    /*asm volatile(
        "movl %0, %%esp\n"
        "movl %0, %%ebp\n"
        :
        : "r" (stack)
    );*/

    /*if(debug) {
        video->kprintf("Entry: %x\n", program);
        for(;;) {
            asm("hlt");
        }
    }*/

    /*for(;;) {
        asm("hlt");
    }*/

    debug = 1;
    jmp_usrmode((entry_point)program);

    asm volatile (   
        "push %%eax\n"
        "push %%ebx\n"
        "push %%ecx\n"
        "push %%edx\n"
        "push %%edi\n"
        "push %%esi\n"
        "movl %1, %%esp\n"   
        "movl %1, %%ebp\n"
        "jmp *%0"               
        : 
        : "r" (program), "r" (stack)
    );
}

void _exit() {
    asm volatile(
        "sti\n"
        "pop %%esi\n"
        "pop %%edi\n"
        "pop %%edx\n"
        "pop %%ecx\n"
        "pop %%ebx\n"
        "pop %%eax\n"
        "mov $0x10, %%ax\n"
        "mov %%ax, %%ds\n"
        "mov %%ax, %%es\n"
        :
        :
        : "eax", "ebx", "ecx", "edx", "esi", "edi"
    );

    fs->open("/home/shell.ndf");

    for(;;) {
        asm("hlt");
    }
}