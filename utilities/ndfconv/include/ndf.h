#ifndef _NDF_H_
#define _NDF_H_

#include "int.h"

#define EXECUTIBLE      0x77
#define SYSTEM          0x78
#define DRIVER          0x79

typedef struct ndf_header {
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
} ndf_header;

typedef struct RelTable {
    u32* entry;
    u32 entrysz;
} RelTable;

#endif