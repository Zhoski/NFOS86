#include <drivers/video/video.h>
#include <cpu/registers.h>
#include <lib/int.h>

#define DF  8
#define GP  13
#define PF  14

typedef struct {
    u32 cr4; u32 cr3; u32 cr2; u32 cr0;
    u32 edi; u32 esi; u32 ebp; u32 blank; u32 ebx; u32 edx; u32 ecx; u32 eax;
    u32 fn; u32 errc; u32 eip; u32 cs; u32 efl; u32 esp; u32 ss;
} __attribute__((packed)) stack;

void c_faults_handler(stack* st) {
    asm volatile("cli");
    video->clear_screen(0);
    switch (st->fn)
    {
        case DF: video->kprintf("Double Fault #DF\n"); break;
        case GP: video->kprintf("General Protection Fault #GP\n"); break;
        case PF: video->kprintf("Page Fault #PF\n"); break;
        default: break;
    }

    video->kprintf("\nEAX: %x\nEBX: %x\nECX: %x\nEDX: %x\n", st->eax, st->ebx, st->ecx, st->edx);
    video->kprintf("ESI: %x\nEDI: %x\nESP: %x\nEBP: %x\n", st->esi, st->edi, st->esp, st->ebp);
    video->kprintf("ERC: %x\nEIP: %x\nCS: %x\nEFL: %x\n", st->errc, st->eip, st->cs, st->efl);
    video->kprintf("ESP: %x\nSS: %x\n", st->esp, st->ss);
    //video->kprintf("ESP: %x\nSS: %x\n", st->esp, st->ss);
    //video->kprintf("ESI: %x\nEDI: %x\n", st->esi, st->edi);
    video->kprintf("CR0: %x\nCR2: %x\nCR3: %x\nCR4: %x\n", st->cr0, st->cr2, st->cr3, st->cr4);
    //video->kprintf("CR0: %x\n", st->cr0);
    for(;;) {
        asm("hlt");
    }
}