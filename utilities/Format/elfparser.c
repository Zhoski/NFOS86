#include "include/elfparser.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <memory.h>

RelTable get_rel_table(Elf32 elf32) {
    RelTable rel_table;

    Elf32_Shdr* elf32_shdr_head = (Elf32_Shdr*)(elf32.src + elf32.elf32_ehdr->e_shoff + elf32.elf32_ehdr->e_shentsize);

    /*printf("FILE TYPE: %d\n", elf32.elf32_ehdr->e_type);
    printf("E_SHNUM: %d\n", elf32.elf32_ehdr->e_shnum);*/

    for(u32 i = 1; i < elf32.elf32_ehdr->e_shnum; i++) {
        elf32_shdr_head = (Elf32_Shdr*)(elf32.src + elf32.elf32_ehdr->e_shoff + elf32.elf32_ehdr->e_shentsize * i);
        /*printf("Section %d\n", i);
        printf("    NAME: %s\n", elf32.string_table + elf32_shdr_head->sh_name);
        printf("    TYPE: %d\n", elf32_shdr_head->sh_type);
        printf("    FLAGS: %x\n", elf32_shdr_head->sh_flags);
        printf("    OFFSET: %d\n", elf32_shdr_head->sh_offset);
        printf("    SIZE: %d\n", elf32_shdr_head->sh_size);*/

        if(elf32_shdr_head->sh_type == SHT_REL) {
            u32 elf32_rel_total = elf32_shdr_head->sh_size / sizeof(Elf32_Rel);
            Elf32_Rel* elf32_rel = (Elf32_Rel*)(elf32.src + elf32_shdr_head->sh_offset);

            rel_table.src = malloc(elf32_rel_total * 4);
            rel_table.entry = elf32_rel_total;

            for(u32 k = 0; k < elf32_rel_total; k++) {
                elf32_rel = (Elf32_Rel*)(elf32.src + elf32_shdr_head->sh_offset + 8 * k);

                rel_table.src[k] = elf32_rel->r_offset;

                //printf("        OFFSET: %d  Rel Type: %d\n", elf32_rel->r_offset, (u8)(elf32_rel->r_info & 0xFF));
            }
        }
    }
    return rel_table;
}

Elf32 get_elf(u8* file) {
    Elf32 elf32;

    FILE* elf32_file = fopen(file, "r");
    if(!elf32_file) {
        printf("File %s not found", file);
        return elf32;
    }

    fseek(elf32_file, 0, SEEK_END);
    u32 elf32_size = ftell(elf32_file);
    fseek(elf32_file, 0, SEEK_SET);

    u8* elf32_src = malloc(elf32_size);
    fread(elf32_src, 1, elf32_size, elf32_file);

    fclose(elf32_file);

    elf32.elf32_ehdr = (Elf32_Ehdr*)(elf32_src);

    Elf32_Shdr* elf32_shdr = (Elf32_Shdr*)(elf32_src + elf32.elf32_ehdr->e_shoff);

    Elf32_Shdr* elf32_string_table_sec = (Elf32_Shdr*)(elf32_src + elf32.elf32_ehdr->e_shoff + elf32.elf32_ehdr->e_shstrndx * elf32.elf32_ehdr->e_shentsize);
    
    u8* string_table = (u8*)(elf32_src + elf32_string_table_sec->sh_offset);

    elf32.src = elf32_src;
    elf32.string_table = string_table;

    return elf32;
}