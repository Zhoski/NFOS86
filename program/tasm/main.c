#include <slib/stdio.h>
#include <slib/types.h>
#include <slib/memory.h>
#include "include/lexer.h"
#include "include/parser.h"

#define HEADER  "\ntasm compiler v 0.1\n"

Token* list;

int main(u32 argc, u8* argv[]) {
    printf(HEADER);
    printf("Compiling %s to %s\n", argv[1], argv[2]);

    Token* list = (Token*)malloc(sizeof(Token) * 4);

    tokenize(argv[1], &list);

    parser(list, argv[2]);

    //printf("%x", (u32)malloc(sizeof(Token) * 4));
    asm("int $0x40"::"a"(0));
}