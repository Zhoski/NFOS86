#include "include/parser.h"
#include <slib/stdio.h>
#include <slib/sys.h>
#include <slib/memory.h>
#include <slib/string.h>
#include "include/opcode.h"
#include "include/ndf.h"

#define FIRST       1
#define GEN_CODE    2

u32 tk;
u32 dst_f_sz;
u32 pointer;
u8* dst_buff;

u32 mk_modrm(u8 mod, u8 reg, u8 rm) {
    return mod << 6 | reg << 3 | rm;
}

u32 get_instr_reg(Token tk) {
    if(strcmp("eax", tk.txt) == 0) return 0;
    if(strcmp("ecx", tk.txt) == 0) return 1;
    if(strcmp("edx", tk.txt) == 0) return 2;
    if(strcmp("ebx", tk.txt) == 0) return 3;
}

void instr_int(Token* list, u32 flag) {
    if(flag == FIRST) dst_f_sz+=2;
    else if(flag == GEN_CODE) {
        *(u8*)(dst_buff + pointer) = INTimm8;
        pointer++;
        *(u8*)(dst_buff + pointer) = list[tk+1].integer;
        pointer++;
    }

    tk+=2;
}

void instr_mov(Token* list, u32 flag) {
    switch (list[tk+1].type)
    {
    case REG32:
        switch (list[tk+3].type)
        {
        case DDWORD:
            if(flag == FIRST) dst_f_sz+=5;
            else if(flag == GEN_CODE) {
                *(u8*)(dst_buff + pointer) = MOVimm32 + get_instr_reg(list[tk+1]);
                pointer++;
                *(u32*)(dst_buff + pointer) = list[tk+3].integer;
                pointer+=4;
            }
            break;
        case REG32:
            if(flag == FIRST) dst_f_sz+=2;
            else if(flag == GEN_CODE) {
                *(u8*)(dst_buff + pointer) = MOVrm32;
                pointer++;
                u32 first_opp = get_instr_reg(list[tk+1]);
                u32 sec_opp = get_instr_reg(list[tk+3]);
                *(u8*)(dst_buff + pointer) = mk_modrm(MODrmreg, first_opp, sec_opp);
                pointer++;
            }
            break;
        
        default:break;
        }
        break;
    
    default:break;
    }

    tk+=4;
}

void pars_loop(Token* list, u32 flag) {
    while (list[tk].type != EOF)
    {
        //printf("tk: %d  type: %d\n", tk, list[tk].type);
        switch (list[tk].type)
        {
            case MOV: instr_mov(list, flag); break;
            case INT: instr_int(list, flag); break;
            default: tk++; break;
        }
    }
}

u32 parser(Token* list, u8* f) {
    tk = 0; dst_f_sz = 0; pointer = 0;

    pars_loop(list, FIRST);

    sys_create(f, 0);
    dst_buff = malloc(dst_f_sz + sizeof(ndf_header_t));

    ndf_header_t header = {
        .entry = 0,
        .fsize = dst_f_sz + sizeof(ndf_header_t),
        .info = 0,
        .infosz = 0,
        .magic[0] = 0x77,
        .magic[1] = 'N',
        .magic[2] = 'D',
        .magic[3] = 'F',
        .reloff = 0,
        .ver_maj = 0,
        .ver_min = 1,
    };

    memcpy(&header, dst_buff, sizeof(ndf_header_t));
    pointer+=sizeof(ndf_header_t);

    tk = 0;
    pars_loop(list, GEN_CODE);

    sys_write(f, dst_buff, dst_f_sz + sizeof(ndf_header_t));
}