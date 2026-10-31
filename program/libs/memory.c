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

unsigned char* malloc(unsigned int n) {
    asm volatile(
        "movl $4, %%eax\n"
        "movl $1, %%ebx\n"
        "movl %0, %%ecx\n"
        "int $0x40"
        :
        : "r"(n)
        : "eax", "ebx", "ecx", "memory"
    );

    unsigned int free_adres;

    asm volatile (
        "movl %%eax, %0"
        : "=r" (free_adres)
        :
        : "memory"
    );

    return (unsigned char*)free_adres;
}

void free(void* ptr) {
    asm volatile(
        "movl $4, %%eax\n"
        "movl $2, %%ebx\n"
        "movl %0, %%ecx\n"
        "int $0x40"
        :
        : "r"((unsigned int)ptr)
        : "eax", "ebx", "ecx", "memory"
    );
}