#include <slib/stdio.h>
#include <slib/types.h>
#include <slib/memory.h>

void main(u32 argc, u8* argv[]) {
    clear_screen(0);
    printf("Hello startup %d\n", 65);
    init_heap();
    u8* txt = malloc(32);
    printf("addr: %x\n", txt);

    //printf("%s", argv[0]);

    //u32* bad = (u32*)0x10001;
    //*bad = 3;

    //asm("hlt");

    //for(u32 i = 0; i < 128; i++) {
        //printf("%x ", *(u8*)(esp + i));
    //}

    //unsigned int* w = (unsigned int*)0x200000;
    //*w = 3;

    //asm("hlt");

    //unsigned int* mops = (unsigned int*)0x500000;
    //*mops = 3;

    //asm("hlt");
    
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
