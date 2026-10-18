#ifndef ELF_PARSER_H
#define ELF_PARSER_H

#include <elf.h>
#include <stdio.h>
#include "type.h"

typedef struct {
    Elf32_Ehdr* elf32_ehdr;
    Elf32_Shdr* elf32_shdr;
    
    u8* string_table;
    u8* src;
} Elf32;

typedef struct
{
    u32* src;
    u32 entry;
} RelTable;

RelTable get_rel_table(Elf32 elf32);

Elf32 get_elf(u8* file);

#endif