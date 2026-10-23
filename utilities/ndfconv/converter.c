#include "include/converter.h"
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>

#define ETN_VER_MAJ     1
#define ETN_VER_MIN     0

Elf32 elf32;

Elf32 get_elf(u8* elf) {
    Elf32 elf32;
    FILE* file = fopen(elf, "r");
    if(!file) {
        printf("[ FAIL ] File %s not found\n", elf);
        return elf32;
    }

    fseek(file, 0, SEEK_END);
    u32 fsz = ftell(file);
    fseek(file, 0, SEEK_SET);

    elf32.src = malloc(fsz);

    fread(elf32.src, 1, fsz, file);

    elf32.ehdr = (Elf32_Ehdr*)elf32.src;

    elf32.shdr = elf32.src + elf32.ehdr->e_shoff;

    Elf32_Shdr* str_shdr = (Elf32_Shdr*)(elf32.src + elf32.ehdr->e_shoff + elf32.ehdr->e_shstrndx * elf32.ehdr->e_shentsize);
    elf32.strtable = (u8*)(elf32.src + str_shdr->sh_offset);

    fclose(file);

    return elf32;
}

u32 convert_to_ndf(u8* elf, u8* out) {
    Elf32 elf32 = get_elf(elf);
    RelTable relt;

    printf("--- ELF ---\n");
    printf("File: %s\n", elf);
    printf("Ident: %s\n", elf32.ehdr->e_ident);
    printf("Type: %d\n", elf32.ehdr->e_type);
    printf("Entry: %d\n", elf32.ehdr->e_entry);
    printf("Shoff: %d\n", elf32.ehdr->e_shoff);
    printf("Shnum: %d\n", elf32.ehdr->e_shnum);
    printf("Shstrndx: %d\n", elf32.ehdr->e_shstrndx);

    printf("--- SECTIONS ---\n");
    for(u32 k = 0; k < elf32.ehdr->e_shnum; k++) {
        Elf32_Shdr* shdr_head = (Elf32_Shdr*)(elf32.src + elf32.ehdr->e_shoff + elf32.ehdr->e_shentsize * k);
        printf("SECTION %d\n", k);
        printf("    name: %s\n", elf32.strtable + shdr_head->sh_name);
        printf("    type: %d\n", shdr_head->sh_type);
        printf("    offset: %d\n", shdr_head->sh_offset);
        printf("    size: %d\n", shdr_head->sh_size);
        printf("    flag: %x\n", shdr_head->sh_flags);

        if(shdr_head->sh_type == SHT_REL) {
            Elf32_Rel* elf32_rel = shdr_head->sh_offset;
            u32 total = shdr_head->sh_size / sizeof(Elf32_Rel);
            relt.entrysz = total * 4;
            relt.entry = malloc(relt.entrysz);

            printf("    Total: %d\n", total);
            for(u32 i = 0; i < total; i++) {
                elf32_rel = (Elf32_Rel*)(elf32.src + shdr_head->sh_offset + 8 * i);
                relt.entry[i] = elf32_rel->r_offset + sizeof(ndf_header);
                printf("        Offset: %d      Type: %d\n", elf32_rel->r_offset, (u8)(elf32_rel->r_info & 0xFF));
            }
        }
    }

    ndf_header header;
    header.magic[0] = EXECUTIBLE;
    header.magic[1] = 'N';
    header.magic[2] = 'D';
    header.magic[3] = 'F';
    header.entry = elf32.ehdr->e_entry;
    header.ver_maj = ETN_VER_MAJ;
    header.ver_min = ETN_VER_MIN;

    char* cmd = malloc(18 + strlen(elf) + strlen(out) + 1);
    strcpy(cmd, "objcopy -O binary ");
    strcat(cmd, elf);
    strcat(cmd, " ");
    strcat(cmd, out);

    system(cmd);

    FILE* out_ndf = fopen(out, "r+b");
    if(!out_ndf) {
        return 0;
    }

    fseek(out_ndf, 0, SEEK_END);
    u32 binary_size = ftell(out_ndf);
    header.reloff = binary_size + sizeof(header);
    header.relsz = relt.entrysz;
    header.fsize = binary_size + sizeof(header) + relt.entrysz;
    fseek(out_ndf, 0, SEEK_SET);

    u8* out_ndf_buffer = malloc(sizeof(ndf_header) + binary_size + relt.entrysz);

    memcpy(out_ndf_buffer, &header, sizeof(ndf_header));
    fread(out_ndf_buffer + sizeof(ndf_header), 1, binary_size, out_ndf);
    memcpy(out_ndf_buffer + sizeof(ndf_header) + binary_size, relt.entry, relt.entrysz);

    fseek(out_ndf, 0, SEEK_SET);
    fwrite(out_ndf_buffer, 1, binary_size + sizeof(ndf_header) + relt.entrysz, out_ndf);
    fclose(out_ndf);

    printf("NDF SIZE: %d\n", binary_size + sizeof(ndf_header) + relt.entrysz);
    printf("RELLOF: %x\n", binary_size + sizeof(ndf_header));
}