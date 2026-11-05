#include <kernel/syscall.h>
#include <kernel/memory.h>
#include <kernel/device.h>
#include <kernel/memory.h>
#include <kernel/loader.h>
#include <drivers/video/video.h>
#include <fs/fs.h>

void syscall_handler(syscall_ct* ct) { 
    asm volatile("sti");
    switch (ct->eax)
    {
    case  SYS_EXIT:
        asm volatile (
            "movl %[stack_top], %%esp\n"
            "movl %[stack_base], %%ebp\n"
            "jmp *%[exit]"
            :
            : [exit] "r"(kernel_return_ptr), [stack_top] "r"(kernel_stack_ptr), [stack_base] "r" (kernel_stack_base)
            : "memory"
        );
        break;
    case WRITE:
        switch (ct->ebx)
        {
            case SCREEN: 
                if(ct->edx == STR) video->kprintf("%s", ct->ecx); 
                else if(ct->edx == SYM) video->kputc((u8)ct->ecx);
                else if(ct->edx == HEX) video->kprintf("%x", ct->ecx);
                break;
            case COM_PORT: break;
        }
        break;
    case KEYBOARD:
        switch (ct->ebx)
        {
            case NO_WAIT: ct->eax = kb.get_last_key(); break;
            case WAIT: break;
        }
        break;
    case MALLOC: 
        ct->eax = (u32)malloc(ct->ebx); 
        break;
    case FREE:   
        free((u8*)ct->ebx); 
        break;
    case SYS_OPEN: {
        u32 status = fs->check((u8*)ct->ebx);
        if(status) {
            u8* descriptor = (u8*)malloc(g_node.size + 5);
            u32* fsize = (u32*)descriptor;
            *fsize = g_node.size;
            fs->read((u8*)ct->ebx, (u8*)(descriptor + 4), *fsize);
            ct->eax = (u32)descriptor;
        } else ct->eax = 0;
        break;
    }
    case SYS_CLOSE:
        free((u8*)ct->ebx);
        break;
    case SYS_CREATE:
        ct->eax = fs->create((u8*)ct->ebx, (u8)ct->ecx);
        break;
    case SYS_DELETE:
        ct->eax = fs->delete((u8*)ct->ebx);
        break;
    case SYS_RUN:
        pr_execute((u8*)ct->ebx, ct->ecx, (u8**)ct->edx);
        break;
    case SET_BG:
        video->terminal_bg_vbe_set((u8)ct->ebx);
        break;
    case SET_FG:
        video->terminal_fg_vbe_set((u8)ct->ebx);
        break;
    case CLEAR:
        video->clear_screen((u8)ct->ebx);
        break;
    case SET_CURSOR:
        video->terminal_set_cursor_position((u16)(ct->ebx >> 16), (u16)(ct->ebx));
        break;
    case GET_CURSOR:
        video->terminal_get_cursor_position((u16*)(ct->ecx), (u16*)ct->edx);
        break;
    case SYS_WRITE:
        ct->eax = fs->update((u8*)ct->ebx, (u8*)ct->edx, ct->ecx);
        break;
    case WRITE_DIR:
        fs->write_dir((u8*)ct->ebx);
        break;
    default:
        break;
    }

    asm volatile(""::"a"(ct->eax));
}