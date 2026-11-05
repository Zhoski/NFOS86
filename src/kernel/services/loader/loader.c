#include <kernel/loader.h>
#include <drivers/video/video.h>
#include <kernel/program.h>
#include <kernel/memory.h>
#include <fs/fs.h>
#include <lib/string.h>

uint32_t kernel_stack_ptr;
uint32_t kernel_stack_base;
uint32_t kernel_return_ptr;

u32 debug = 0;

typedef void (*entry_point)();

void _exit();
extern void jmp_usrmode(u32 entry, u32 stack);

#define ENTRY       (void*)0x300000
#define STACK       (void*)0x3FFFF0

u32 program;

u32 g_argc;
u8** g_argv;

void patch(void* entry, void* stack) {
    ndf_header_t ndf_header;
    memcpy(entry, &ndf_header, sizeof(ndf_header));
    
    u32 entry_addr = (u32)entry;

    memcpy((void*)(entry_addr + sizeof(ndf_header_t)), (void*)(entry_addr), ndf_header.fsize - sizeof(ndf_header_t));
    
    u32* patch_offset = (u32*)(entry_addr + ndf_header.reloff - sizeof(ndf_header_t));
    for(u32 k = 0; k < ndf_header.relsz / 4; k++) {
        u32* addr_patch = (u32*)(entry_addr + patch_offset[k] - sizeof(ndf_header_t));
        *addr_patch = *addr_patch + entry_addr;
    }

    program = (u32)(entry_addr + ndf_header.entry - sizeof(ndf_header_t) + 28);
}

u32 pr_execute(u8* f, u32 argc, u8* argv[]) {
    if(!fs->check(f)) {
        return 0;
    }

    g_argc = argc;
    g_argv = argv;

    //memset((void*)0x300000, 0, 0xFFFFF);

    fs->read(f, (u8*)ENTRY, g_node.size);

    patch((void*)ENTRY, (void*)STACK);

    kernel_return_ptr = (uint32_t)&_exit;
    asm volatile("movl %%esp, %0":"=r" (kernel_stack_ptr));
    asm volatile("movl %%ebp, %0":"=r" (kernel_stack_base));

    u32 offset = 0;

    for(u32 i = 0; i < argc; i++) {
        u32 arg_len = strlen(argv[i]) + 1;
        offset += arg_len;
        memcpy(argv[i], STACK - offset, arg_len);
        g_argv[i] = STACK - offset;
        offset += offset % 4;
    }

    for(u32 i = argc; i > 0; i--) {
        offset += 4;
        u32* push_add = (u32*)(STACK - offset);
        *push_add = (u32)g_argv[i - 1];
    }

    g_argv = (u8**)(STACK - offset);

    offset += 4;
    u32* push_add = (u32*)(STACK - offset);
    *push_add = (u32)g_argv;
    offset += 4;
    push_add = (u32*)(STACK - offset);
    *push_add = argc;

    jmp_usrmode(program, (u32)STACK - offset - 4);
}

void _exit() {
    pr_execute("/apps/shell.ndf",0,0);

    for(;;) {
        asm("hlt");
    }
}
