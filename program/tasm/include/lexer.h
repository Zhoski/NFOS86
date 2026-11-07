#ifndef TOKENIZE_H
#define TOKENIZE_H

#include <slib/types.h>

typedef enum {
    EOF,
    MOV,
    INT,
    COMMA,
    REG32,
    LABEL,
    DDWORD,
    NL,
} TokenType;

typedef struct {
    u8* txt;
    TokenType type;
} keywords_t;

static keywords_t keywords[] = {
    {.txt = "eof", .type = EOF},
    {.txt = "mov", .type = MOV},
    {.txt = "int", .type = INT},
    {.txt = ",", .type = COMMA},
    {.txt = "eax", .type = REG32},
    {.txt = "ebx", .type = REG32},
    {.txt = "ecx", .type = REG32},
    {.txt = "edx", .type = REG32},
};

typedef struct {
    TokenType type;
    u32 integer;
    u8* txt;
} Token;

typedef struct {
    u8* src;
    u8  sym;
    u32 off;
} Lexer;

u32 tokenize(u8* f, Token** list);

#define KEYWOARDS_NUM sizeof(keywords) / sizeof(keywords_t)

#endif