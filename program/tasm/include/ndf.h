#ifndef NDF_H
#define NDF_H

#include <slib/types.h>

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
} ndf_header_t;

#endif