#include <stdio.h>
#include "include/converter.h"

int main(int argc, char* argv[]) {
    printf("ndfconv v 0.1\n");
    //printf("%s", argv[1]);
    return convert_to_ndf(argv[1], argv[2]);
}