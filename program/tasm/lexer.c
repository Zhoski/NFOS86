#include "include/lexer.h"
#include <slib/types.h>
#include <slib/stdio.h>
#include <slib/sys.h>
#include <slib/memory.h>
#include <slib/string.h>

#define FILE_NOT_FOUND  0x1

Lexer lexer;

u32 src_f_sz = 0;
u32 dest_f_sz = 0;

void lexer_move(u32 n) {
    lexer.off += n;
    lexer.sym = *(u8*)(lexer.src + lexer.off);
}

void skip_space() {
    while (lexer.sym == ' ') lexer_move(1);
}

Token get_next_token() {
    Token tk = {
        .integer = 0,
        .txt = 0,
        .type = EOF,
    };
    if(lexer.off < src_f_sz) {
        skip_space(lexer);
        if(lexer.sym == ',') {
            tk.type = COMMA;
            tk.txt = ",";
            lexer_move(1);
            return tk;
        }

        if(lexer.sym == '\n') {
            tk.type = NL;
            lexer_move(1);
            return tk;
        }

        if(lexer.sym >= '0' && lexer.sym <= '9') {
            tk.type = DDWORD;
            tk.integer = stoi((lexer.src + lexer.off));
            while (lexer.sym >= '0' && lexer.sym <= '9') lexer_move(1);
            return tk;
        }

        for(u32 i = 0; i < KEYWOARDS_NUM; i++) {
            u32 keywoards_len = strlen(keywords[i].txt);
            if(memcmp(lexer.src + lexer.off, keywords[i].txt, keywoards_len)) {
                tk.type = keywords[i].type;
                tk.txt = malloc(keywoards_len + 1);
                memcpy(keywords[i].txt, tk.txt, keywoards_len);
                lexer_move(keywoards_len);
                return tk;
            }
        }

        printf("Undefied instruction: %s\n", (lexer.src + lexer.off));
        for(;;);
    }

    return tk;
}

u32 tokenize(u8* f, Token** list) {
    u32 fd = sys_open(f);
    if(!fd) {
        return FILE_NOT_FOUND;
    }

    src_f_sz = *(u32*)fd;

    lexer.src = (u8*)(fd + 4);
    lexer.off = 0;
    lexer.sym = *lexer.src;

    u32 tk = 0;

    while (1)
    {
        if(tk % 4 == 0 && tk != 0) {
            Token* nlist = malloc(sizeof(Token) * tk + sizeof(Token) * 4);
            memcpy(*list,nlist,sizeof(Token) * tk);
            free(*list);
            *list = nlist;
        }

        (*list)[tk] = get_next_token();
        if((*list)[tk].type == EOF) break;
        
        tk++;
    }
}