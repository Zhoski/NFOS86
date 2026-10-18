#include "include/tondf.h"
#include "include/elfparser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXECUTABLE  0x01
#define SYSTEM      0x02
#define DRIVER      0x03

typedef struct ndf_header {
    u8 mag[3];
    u8 type;
    u32 entryoff;
    u32 fsize;
    u32 rellof;
    u32 relsize;
    u32 inoff;
    u32 insize;
} ndf_header_t;

void elf_to_ndf(request_t* req) {
    Elf32 elf32 = get_elf(req->elf_file);

    RelTable rel = get_rel_table(elf32);

    ndf_header_t ndf_header;
    ndf_header.mag[0] = 'N';
    ndf_header.mag[1] = 'D';
    ndf_header.mag[2] = 'F';

    ndf_header.type = EXECUTABLE;

    ndf_header.entryoff = sizeof(ndf_header_t);

    char* cmd = malloc(18 + strlen(req->elf_file) + strlen(req->out) + 1);
    strcpy(cmd, "objcopy -O binary ");
    strcat(cmd, req->elf_file);
    strcat(cmd, " ");
    strcat(cmd, req->out);

    system(cmd);

    FILE* ndf_file = fopen(req->out, "r+b");

    if(!ndf_file) {
        return;
    }

    fseek(ndf_file, 0, SEEK_END);
    u32 ndf_size = ftell(ndf_file);
    u32 bin_size = ndf_size;
    fseek(ndf_file, 0, SEEK_SET);

    ndf_header.rellof = sizeof(ndf_header_t) + bin_size;
    ndf_header.relsize = rel.entry * 4;

    ndf_size += sizeof(ndf_header_t);
    ndf_size += rel.entry * 4;

    ndf_header.fsize = ndf_size;

    u8* ndf_buffer = malloc(ndf_size);
    memcpy(ndf_buffer, &ndf_header, sizeof(ndf_header_t));
    fread(ndf_buffer + sizeof(ndf_header_t), 1, bin_size, ndf_file);
    memcpy(ndf_buffer + sizeof(ndf_header_t) + bin_size, &rel, rel.entry * 4);

    fseek(ndf_file, 0, SEEK_SET);
    fwrite(ndf_buffer, 1, ndf_size, ndf_file);

    fclose(ndf_file);

    printf("%d", bin_size);
}