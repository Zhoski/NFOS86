#include <slib/file.h>

unsigned int sys_read(const unsigned char* __restrict__ file_name, unsigned int n, unsigned char* __restrict__ out) {
    unsigned int ret = 0;
    asm volatile(
        "int $0x80"
        : "=a" (ret)
        : "a" (5), "b" (1), "c" (n), "S" (file_name), "d" (out)
        : "memory"
    );
    return ret;
}

unsigned int sys_check(const unsigned char* __restrict__ file_name) {
    unsigned int ret = 0;
    asm volatile(
        "int $0x80"
        : "=a" (ret)
        : "a" (5), "b" (5), "S" (file_name)
        : "memory"
    );
    return ret;
}

unsigned int sys_write(const unsigned char* __restrict__ file_name, unsigned char* __restrict__ in, unsigned int count) {
    int ret = 0;
    asm volatile(
        "movl $5, %%eax\n"
        "movl $2, %%ebx\n"
        "movl %[c], %%ecx\n"
        "movl %[file], %%esi\n"
        "movl %[in], %%edi\n"
        "int $0x80"
        :
        : [c] "m" (count), [file] "m" ((int)file_name), [in] "m" ((int)in)
        : "eax", "ebx", "ecx", "esi", "edi", "memory"
    );
    asm volatile(
        "movl %0, %%eax"
        : "=a" (ret)
    );
}

unsigned int sys_create(const unsigned char* __restrict__ file_name, unsigned char flag) {
    unsigned int ret = 0;
    asm volatile(
        "int $0x80"
        : "=a" (ret)
        : "a" (5), "b" (3), "c"(flag), "S" (file_name)
        : "memory"
    );
    return ret;
}

unsigned int sys_delete(const unsigned char* __restrict__ node) {
    int ret = 0;
    asm volatile(
        "int $0x80"
        : "=a" (ret)
        : "a" (5), "b" (4), "S" (node)
        : "memory"
    );

    return ret;
}

unsigned int write_root(unsigned char *__restrict__ dir) {
    unsigned int ret = 0;
    asm volatile(
        "int $0x80"
        : "=a" (ret)
        : "a" (5), "b" (6), "S" (dir)
        : "memory"
    );

    return ret;
}

unsigned int sys_run(const unsigned char* __restrict__ file) {
    int ret = 0;
    asm volatile(
        "int $0x80"
        : "=a" (ret)
        : "a" (6), "b" (1), "S" (file)
        : "memory"
    );

    return ret;
}

unsigned int sys_died() {
    asm volatile(
        "movl $0x06, %%eax\n"
        "movl $0x60, %%ebx\n"
        "int $0x80"
        :
        : 
    );
}

File* fopen(unsigned char *__restrict__ file, const unsigned int mode) {
    File* ret;

    asm volatile(
        "movl $5, %%eax\n"
        "movl $8, %%ebx\n"
        "movl %0, %%esi\n"
        "int $0x80"
        :
        : "r"((unsigned int)file)
        : "eax", "ebx", "esi", "memory"
    );

    asm volatile(
        "movl %0, %%eax"
        : "=a" (ret)
    );

    unsigned int i = 0;
    while (file[i])
    {
        ret->name[i] = file[i];
        i++;
    }

    return ret;
}

void fclose(File* file) {
    asm volatile(
        "movl $4, %%eax\n"
        "movl $2, %%ebx\n"
        "movl %[file], %%esi\n"
        "int $0x80"
        :
        : [file] "r"(file)
        : "eax", "ebx", "esi", "memory"
    );
}

void fread(File* stream, unsigned int n, unsigned char* out) {
    asm volatile (
        "movl %[from], %%esi\n"
        "movl %[in],   %%edi\n"
        "movl %[count],%%ecx\n"
        "rep movsb"
        :
        : [from] "r" ((unsigned char*)stream->cur), [in] "r" ((unsigned char*)out), [count] "r" (n)
        : "esi", "edi", "ecx","memory"
    ); 
}

void fwrite(File* stream, unsigned int n, unsigned char* in) {
    asm volatile(
        "movl $5, %%eax\n"
        "movl $2, %%ebx\n"
        "movl %[c], %%ecx\n"
        "movl %[file], %%esi\n"
        "movl %[in], %%edi\n"
        "int $0x80"
        :
        : [c] "m" (n), [file] "m" ((int)stream), [in] "m" ((int)in)
        : "eax", "ebx", "ecx", "esi", "edi", "memory"
    );
}