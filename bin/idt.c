#include <cpu/idt.h>
#include <drivers/video/video.h>

struct InterruptDescriptor32 IDT[256]; // Таблица на 256 прерываний

void idt_load() {
    struct IDT_pointer idt_ptr;
    idt_ptr.limit = (U16)(sizeof(struct InterruptDescriptor32) * 256) - 1;
    idt_ptr.base  = (U32)&IDT;

    asm volatile("lidt (%0)" : : "r"(&idt_ptr));
}

void idt_set(U8 index, U16 sel, U8 access, U32 handler) {
    if(!index) {
        for(;;) {
            asm("hlt");
        }
    }
    IDT[index].base_low = handler & 0xFFFF;
    IDT[index].base_hight = (handler >> 16) & 0xFFFF;

    IDT[index].segment = sel;   
    IDT[index].reserved = 0;       
    IDT[index].access = access;       
}