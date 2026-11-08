#include <slib/stdio.h>

void write_int(unsigned int x) {
    asm volatile (
        "movl $1, %%eax\n"
        "movl $0, %%ebx\n"
        "movl %0, %%ecx\n"
        "movl $2, %%edx\n"
        "int $0x40"
        :
        : "r" (x)
        : "eax", "ebx","ecx","edx"
    );
}

char* oops = "oops";

void main() {
    clear_screen(0);
    printf("Hello startup %d\n", 65);
    unsigned int esp, ebp;
    asm volatile(
        "movl %%esp, %0\n"
        "movl %%ebp, %1\n"
        : "=r"(esp), "=r"(ebp)
    );

    printf(oops);
    printf("%d", 77);
    
    for(;;) {
        char c = get_char();
        if(c) {
            if(c == 0x0A) {
                printf("\n> ");
            }else {
                putchar(c);
            }
        };
    }
    asm("int $0x40"::"a"(0));
} 