#ifndef DEBUG_H
#define DEBUG_H

static inline void qemu_point(char* s) {
    while (*s)
    {
        asm volatile("outb %0, $0xE9"::"a"(s));
        s++;
    }
}

static inline void qemu_stop_point(char* s) {
    while (*s)
    {
        asm volatile("outb %0, $0xE9"::"a"(*s));
        s++;
    }

    for(;;) {
        asm("hlt");
    }
}

static inline void qemu_debug(char* log) {
    while (*log)
    {
        asm volatile("outb %0, $0xE9"::"a"(*log));
        log++;
    }
}

#endif