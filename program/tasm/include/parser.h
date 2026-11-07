#ifndef PASRER_H
#define PASRES_H

#include <slib/types.h>
#include "lexer.h"

typedef struct {
    u8 instruction_prefix;
    u8 addres_size_prefix;
    u8 opernd_size_prefix;
    u8 segment_override;
    u8 opcode[2];
    u8 modrm;
    u8 sib;
    u8 displacement[4];
    u8 immediate[4];
} Instruction;

u32 parser(Token* list, u8* f);

#endif