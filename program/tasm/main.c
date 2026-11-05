#include <slib/stdio.h>
#include <slib/types.h>

#define HEADER  "\ntasm compiler v 0.1\n"

int main(u32 argc, u8* argv[]) {
    printf(HEADER);
    for(u32 i = 0; i < argc; i++) {
        printf("arg %d: %s\n", i, argv[i]);
    }
    for(;;);
    asm("int $0x40"::"a"(0));
}