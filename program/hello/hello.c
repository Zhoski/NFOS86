#include <slib/stdio.h>

char* oops = "oops";

void main() {
    clear_screen(0);
    printf("Hello startup %d\n", 65);

    unsigned int* w = (unsigned int*)0x1FFF;
    *w = 3;

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
