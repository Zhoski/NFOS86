#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdint.h>

typedef struct
{
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint32_t eax;
    uint32_t edi;

} __attribute__((packed)) Regs;

#endif