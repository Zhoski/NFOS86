#ifndef LOADER_H
#define LOADER_H

#include <lib/int.h>

extern uint32_t kernel_stack_ptr;
extern uint32_t kernel_stack_base;
extern uint32_t kernel_return_ptr;

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

u32 execute(u8* f, u32 argc, u8* argv[]);

#endif
