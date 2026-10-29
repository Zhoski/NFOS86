#include <drivers/video/video.h>
#include <cpu/registers.h>
#include <lib/int.h>

#define DF  8
#define GP  13
#define PF  14

void c_faults_handler(u32 fn) {
    video->clear_screen(0);
    switch (fn)
    {
        case GP: video->kprintf("General Protection Fault #GP\n"); break;
        case PF: video->kprintf("Page Fault #PF\n"); break;
        default: break;
    }
    
    for(;;) {
        asm("hlt");
    }
}