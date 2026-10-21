#include <kernel/loader.h>
#include <drivers/video/video.h>
#include <kernel/program.h>
#include <kernel/memory.h>

typedef void (*entry_point)();

void exit();

void execute_file(void* entry, void* stack, ext_t ext) {
    ndf_header_t* ndf_header = (ndf_header_t*)entry;
    u32 entry_addr = (u32)entry;
    u32* patch_offset = (u32*)(entry_addr + ndf_header->reloff);
    for(u32 k = 0; k < ndf_header->relsz / 4; k++) {
        u32* addr_patch = (u32*)(entry_addr + patch_offset[k]);
        video->kprintf("[ INFO ] Patching addr: %x  Offset: %x  Data: %x\n", addr_patch, patch_offset[k], *addr_patch);
        *addr_patch = *addr_patch + entry_addr;
    }

    kernel_return_ptr = (uint32_t)&exit;
   
    entry_point program = (entry_point)((u32)entry + ndf_header->entry - sizeof(ndf_header_t));

    memcpy((void*)(entry_addr + sizeof(ndf_header_t)), (void*)(entry_addr), ndf_header->fsize - sizeof(ndf_header_t));

    asm volatile("movl %%esp, %0":"=r" (kernel_stack_ptr));
    asm volatile("movl %%ebp, %0":"=r" (kernel_stack_base));

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

void exit() {
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

    for(;;) {
        asm("hlt");
    }
}