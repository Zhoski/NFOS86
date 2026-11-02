#include <slib/sys.h>

unsigned int sys_read(const char* __restrict__ f, unsigned int n, unsigned char* dest) {
    unsigned int ret = 0;
    asm volatile(
        "int $0x40"
        : "=a" (ret)
        : "a" (5), "b" (1), "c" (n), "S" (f), "d" (dest)
        : "memory"
    );
    return ret;
}

unsigned int sys_check(const char* f) {
    unsigned int ret = 0;
    asm volatile(
        "int $0x40"
        : "=a" (ret)
        : "a" (5), "b" (5), "S" (f)
        : "memory"
    );
    return ret;
}

unsigned int sys_write(const char* f, unsigned char* src, unsigned int n) {
    int ret = 0;
    asm volatile(
        "movl $5, %%eax\n"
        "movl $2, %%ebx\n"
        "movl %[c], %%ecx\n"
        "movl %[file], %%esi\n"
        "movl %[in], %%edi\n"
        "int $0x40"
        :
        : [c] "m" (n), [file] "m" ((int)f), [in] "m" ((int)src)
        : "eax", "ebx", "ecx", "esi", "edi", "memory"
    );
    asm volatile(
        "movl %0, %%eax"
        : "=a" (ret)
    );
}

unsigned int sys_create(const char* f, unsigned char flag) {
    unsigned int ret = 0;
    asm volatile(
        "int $0x40"
        : "=a" (ret)
        : "a" (5), "b" (3), "c"(flag), "S" (f)
        : "memory"
    );
    return ret;
}

unsigned int sys_delete(const char* node) {
    int ret = 0;
    asm volatile(
        "int $0x40"
        : "=a" (ret)
        : "a" (5), "b" (4), "S" (node)
        : "memory"
    );

    return ret;
}

unsigned int write_root(const char* dir) {
    unsigned int ret = 0;
    asm volatile(
        "int $0x40"
        : "=a" (ret)
        : "a" (5), "b" (6), "S" (dir)
        : "memory"
    );

    return ret;
}

unsigned int sys_run(const char* f) {
    int ret = 0;
    asm volatile(
        "int $0x40"
        : "=a" (ret)
        : "a" (6), "b" (1), "S" (f)
        : "memory"
    );

    return ret;
}

unsigned int sys_died() {
    asm volatile(
        "movl $0x06, %%eax\n"
        "movl $0x60, %%ebx\n"
        "int $0x40"
        :
        : 
    );
}

/**
 *  Возвращает дексриптор файла
 */
unsigned int sys_open(const char* f) {
    
}