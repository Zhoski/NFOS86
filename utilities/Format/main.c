#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "include/elfparser.h"
#include "include/type.h"
#include "include/request.h"
#include "include/tondf.h"

#define FLAG_MAX    1

request_t request;

typedef struct flag {
    u8* flag;
    void (*handler)(char* argv[], u32 argc, u32* idx);
};

void o_flag(char* argv[], u32 argc, u32* idx) {
    request.o = 1;

    request.out = argv[*idx + 1];
    (*idx)++;
}

struct flag f[] = {
    {"-o", o_flag},
};

int main(int argc, char* argv[]) {
    u32 i = 2;
    request.elf_file = argv[1];
    while (i < argc)
    {
        u8 flag_found = 0;
        for(u32 k = 0; k < FLAG_MAX; k++) {
            if(!(strcmp(f[k].flag, argv[i]))) {
                f[k].handler(argv, argc, &i);
                flag_found = 1;
            }
        }

        if(!flag_found) {
            i++;
        }
    }

    elf_to_ndf(&request);
}