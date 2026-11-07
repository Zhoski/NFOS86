#include <slib/stdio.h>
#include <slib/sys.h>
#include <slib/types.h>

int main(u32 argc, u8* argv[]) {
    clear_screen(0);
    
    u32 fd = sys_open(argv[1]);

    if(!fd) {
        printf("file not found\n");
        sys_died();
    }

    void* pointer = (void*)(fd + 4);
    u32 f_sz = *(u32*)fd;
    u32 nl = 0;
    
    for(u32 i = 0; i < f_sz;i++) {
        if(*(u8*)pointer < 0x10) {
            printf("0");
        }
        printf("%x ", *(u8*)pointer);
        nl++;
        if(nl == 24) {
            printf("\n");
            nl = 0;
        }
        pointer++;
    }
    
    printf("\npress any key to return\n");
    u8 c = 0;
    while (!c) c = get_char();

    clear_screen(0);
    sys_died();
}