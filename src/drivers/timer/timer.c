#include <drivers/timer/timer.h>
#include <cpu/cpu.h>
#include <cpu/io.h>
#include <kernel/process.h>

typedef struct {
    u32 es; u32 ds; u32 edi; u32 esi; u32 ebp; u32 blank;
    u32 ebx; u32 ecx; u32 edx; u32 eax;
    u32 eip; u32 cs; u32 efl; u32 esp; u32 ss;
} __attribute__((packed)) stack;

volatile U32 tick = 0;

void init_timer(U32 freequence) {
    U32 divisior = 1193182 / freequence;

    outb(0x43, 0x36);

    U32 low = (U8)(divisior & 0xFF);
    U32 hight = (U8)((divisior >> 8) & 0xFF);

    outb(0x40, low);
    outb(0x40, hight);
}

void ksleep(U32 delay) {
    U32 target = tick + delay;

    while (tick < target)
    {
        halt();
    }
}
#include <drivers/video/video.h>
void timer_handler(stack* st) {
    //video->kprintf("es: %x\n", st->eip);
    if(prcs_start_executing) {
        prcs_tbl[cur_prcs].regs.eax = st->eax;
        prcs_tbl[cur_prcs].regs.ebx = st->ebx;
        prcs_tbl[cur_prcs].regs.ecx = st->ecx;
        prcs_tbl[cur_prcs].regs.edx = st->edx;
        prcs_tbl[cur_prcs].regs.esi = st->esi;
        prcs_tbl[cur_prcs].regs.edi = st->edi;
        prcs_tbl[cur_prcs].regs.esp = st->esp;
        prcs_tbl[cur_prcs].regs.eip = st->eip;

        //execute_next_process();
    }
    tick++;
}
