#ifndef REQUEST_H
#define REQUEST_H

#include "type.h"

typedef struct request request_t;

struct request {
    u8 o:1;     // 0 бит - (-o) выходной файл
    u8 rsv:7;

    u8* elf_file;
    u8* out;
};

#endif