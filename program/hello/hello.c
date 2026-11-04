#include <slib/stdio.h>

void main() {
    printf("\nHello startup");
    asm("int $0x40"::"a"(0));
}