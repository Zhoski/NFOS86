#include <slib/memory.h>

void memset(void* in, unsigned char v, unsigned int c) {
    asm volatile (
        "movl %[v], %%eax\n"
        "movl %[in], %%edi\n"
        "movl %[count], %%ecx\n"
        "rep stosb"
        :
        : [in] "r" ((unsigned char*)in), [v] "r" ((unsigned int)v), [count] "r" (c)
        : "eax", "edi", "ecx", "memory"
    );
}

void memcpy(void* src,void* dest, unsigned int n) {
    asm volatile (
        "movl %[from], %%esi\n"
        "movl %[in],   %%edi\n"
        "movl %[count],%%ecx\n"
        "rep movsb"
        :
        : [from] "r" ((unsigned char*)src), [in] "r" ((unsigned char*)dest), [count] "r" (n)
        : "esi", "edi", "ecx","memory"
    ); 
}

void* malloc(unsigned int n) {
    unsigned int ret;
    asm volatile(
        "int $0x40"
        :  "=a" (ret)
        : "a" (3), "b" (n)
        : "memory"
    );

    return (void*)ret;
}

void free(void* ptr) {
    asm volatile(
        "int $0x40"
        :
        : "a" (4), "b" (ptr)
        : "memory"
    );
}

unsigned int memcmp(void* p1, void* p2, unsigned int n) {
    unsigned int i = 0;
    while (i < n)
    {
        if(*(unsigned char*)(p1 + i) != *(unsigned char*)(p2 + i)) {
            return 0;
        }
        i++;
    }

    return 1;
}