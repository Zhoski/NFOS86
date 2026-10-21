#ifndef _CONVERTER_H_
#define _CONVERTER_H_

#include <elf.h>
#include "ndf.h"
#include "int.h"

typedef struct {
    Elf32_Ehdr* ehdr;
    Elf32_Shdr* shdr;
    
    u8* src;
    u8* strtable;
} Elf32;

u32 convert_to_ndf(u8* elf32, u8* out);

#endif