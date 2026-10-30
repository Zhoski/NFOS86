#ifndef LOADER_H
#define LOADER_H

#include <lib/int.h>

typedef struct {
    u8 magic[4];
    u32 entry;
    u32 fsize;
    u32 reloff;
    u32 relsz;
    u32 info;
    u32 infosz;
    u8 ver_maj;
    u8 ver_min;
    u16 reserved;
} __attribute__((packed)) ndf_header_t;

typedef enum {
    NDF,
} ext_t;

void execute_file(void* entry, void* stack, ext_t ext);

#endif