#include <slib/stdio.h>
#include <slib/string.h>
#include <slib/file.h>
#include <slib/memory.h>
#include <slib/types.h>

#define COMMAND_BUFFER_SIZE    128
#define COMMAND_COUNT           12

#define SHIFT 0x01
#define CAPS  0x02
#define CTRL  0x03
#define ENTER 0x0A 
#define BACKSPACE 0x08 

typedef struct __attribute__((packed)) {
    u8 name[8];
    u8 ext[3];
    u16 start_sec;
    u16 size;
    u8 flags;
} _File;

typedef struct Command
{
    u8* cmd_name;
    u32 (*handler)();
}Cmd;

u32 cmd_help();
u32 cmd_clear();
u32 cmd_dir();
u32 cmd_read();
u32 cmd_write();
u32 cmd_create();
u32 cmd_delete();
u32 cmd_check();
u32 cmd_sys();
u32 cmd_cd();
u32 cmd_mkdir();

struct Command cmd[] = {
    {"help", cmd_help},
    {"clear", cmd_clear},
    {"ls", cmd_dir},
    {"exit", sys_died},
    {"cat", cmd_read},
    {"write", cmd_write},
    {"touch", cmd_create},
    {"rm", cmd_delete},
    {"is",cmd_check},
    {"sys",cmd_sys},
    {"cd", cmd_cd},
    {"mkdir", cmd_mkdir},
};
u32 command_index = 0;
u8 *command_buffer;

u32 *argv_buff;
u8** argv;

u8* info = "Copyright (c) 2026 Zhoski. Licensed under the MIT License.\n"
           "Type 'help' to get a list of commands.\n\n";

u8* cur_dir;

u32 cmd_help() {
    u8* help_msg =   "\n"
                        "[BASE]\n"
                        "   clear                -- clear screen\n"
                        "[DISK]\n"
                        "   touch  [file]        -- creates file\n"
                        "   rm     [file]        -- deletes file\n"
                        "   cat    [file]        -- read file\n"
                        "   write  [file] [data] -- write in file\n"
                        "   mkdir  [dir]         -- creates dir\n"
                        "   ls                   -- displays all files in a directory\n";
    printf(help_msg);
}

u32 cmd_clear() {
    clear_screen(0);

    printf(info);
}

/**
 *  FIX: Исправить на fopen в будущем
 */
u32 cmd_read() {
    putchar('\n');
    /*File* f = fopen(argv[1], FREAD);
    if(!f) {
        printf("%[12Can't open [%s]%[15",argv[1]);
        return;
    }
    if(!f->cur) {
        printf("[File empty]");
    }else {
        u8* buffer = malloc(f->bytes);
        fread(f, f->bytes, buffer);
        buffer[f->bytes] = '\0';
        printf("%s",buffer);
    }
    fclose(f);*/

    u8* full_path = malloc(strlen(cur_dir) + strlen(argv[1]) + 1);
    strcat(full_path, cur_dir);
    strcat(full_path, argv[1]);

    u8* temp_buffer = malloc(4096);
    memset(temp_buffer, 0, 4096);
    u32 read_status = sys_read(full_path, 4096, temp_buffer);

    if(read_status == 0) {
        printf("%s", temp_buffer);
    }else {
        printf("[ %[04FAIL%[07 ] File %s not found", argv[1]);
    }
    free(temp_buffer);

    free(full_path);
}

u32 cmd_check() {
    putchar('\n');

    u8* full_path = malloc(strlen(cur_dir) + strlen(argv[1]) + 1);
    strcat(full_path, cur_dir);
    strcat(full_path, argv[1]);

    printf("Full path: %s\n", full_path);

    if(!sys_check(full_path)) {
        printf("File %s not found", argv[1]);
    }else {
        printf("File %s found", argv[1]);
    }

    free(full_path);
}

/**
 *  FIX: Исправить на fopen в будущем
 */
u32 cmd_write() {
    putchar('\n');
    /*int size = 0;
    File* f = fopen(argv[1], FWRITE);
    if(!f) {
        printf("%[12Can't write [%s]%[15",argv[1]);
        return;
    }else {
        u32 s = 0;
        while (argv[2][s++]);
        
        //sys_write(f->name, argv[2], s);
        fwrite(f, s, argv[2]);
    }
    fclose(f);*/

    u8* full_path = malloc(strlen(cur_dir) + strlen(argv[1]) + 1);
    strcat(full_path, cur_dir);
    strcat(full_path, argv[1]);

    if(sys_write(full_path, argv[2], strlen(argv[2])) == 0x00) {
        printf("[  %[02OK%[07  ] File %s overwrite", argv[1]);
    }else {
        printf("[ %[04FAIL%[07 ] File %s not found", argv[1]);
    }

    free(full_path);
}

u32 cmd_delete() {
    putchar('\n');

    u8* full_path = malloc(strlen(cur_dir) + strlen(argv[1]) + 1);
    strcat(full_path, cur_dir);
    strcat(full_path, argv[1]);

    int ret = sys_delete(full_path);
    if(!ret == 0) {
        printf("[ %[04FAIL%[07 ] File %s not found", argv[1]);
    }else {
        printf("[  %[02OK%[07  ] File %s succes deleted", argv[1]);
    }

    free(full_path);
}

u32 cmd_create() {
    putchar('\n');

    u8* full_path = malloc(strlen(cur_dir) + strlen(argv[1]) + 1);
    strcat(full_path, cur_dir);
    strcat(full_path, argv[1]);

    if(sys_create(full_path, 0) == 0) {
        printf("[  %[02OK%[07  ] File %s succes created", argv[1]);
    } else {
        printf("\n[ %[04FAIL%[07 ] File %s create error", argv[1]);
    }

    free(full_path);
}

u32 cmd_mkdir() {
    putchar('\n');

    u8* full_path = malloc(strlen(cur_dir) + strlen(argv[1]) + 1);
    strcat(full_path, cur_dir);
    strcat(full_path, argv[1]);

    if(sys_create(full_path, 0xFF) == 0) {
        printf("[  %[02OK%[07  ] Dir %s succes created", argv[1]);
    } else {
        printf("\n[ %[04FAIL%[07 ] Dir %s create error", argv[1]);
    }

    free(full_path);
}

u32 cmd_dir() {
    printf("\n");

    printf("%s\n", cur_dir);

    write_root(cur_dir);
}

u32 cmd_sys() {
    printf("\n");
    u16 ram = *(u16*)0x1000;
    u8*  cpu = malloc(48);
    memcpy((void*)0x1008, (void*)cpu, 48);

    printf("\nOS: NFOS86 Demo\n"); 
    printf("Kernel: v0.0.1-custom\n");
    printf("CPU: %s\n", cpu); 
    printf("Memory: %dk\n", ram); 
    printf("Display: VGA 640x480 16 colors\n"); 
    printf("Shell: Default\n");
    printf("FS: FluidFS\n");
    printf("\n%[01\xDB\xDB%[02\xDB\xDB%[03\xDB\xDB%[04\xDB\xDB%[05\xDB\xDB"
        "%[06\xDB\xDB%[07\xDB\xDB%[08\xDB\xDB%[09\xDB\xDB%[10\xDB\xDB%[11\xDB\xDB"
        "%[12\xDB\xDB%[13\xDB\xDB%[14\xDB\xDB%[15\xDB\xDB%[07\n");
}

u32 cmd_cd() {
    if(strcmp(argv[1], "..") != 0) {
        printf("\n");

        u8* full_path = malloc(strlen(cur_dir) + strlen(argv[1]) + 1);
        strcat(full_path, cur_dir);
        strcat(full_path, argv[1]);

        //printf("Full path: %s\n", full_path);

        if(sys_check(full_path)) {
            strcat(cur_dir, argv[1]);
            strcat(cur_dir, "/");

            printf("[%[02  OK  %[07] Directory changed");
        }else {
            printf("[%[04 FAIL %[07] Directory not found");
        }

        free(full_path);
    }else {
        u32 cur_dir_len = strlen(cur_dir);
        if(cur_dir_len > 1) {
            u32 i = cur_dir_len;

            //printf("cur_dir len: %d\n", cur_dir_len);

            cur_dir[i - 1] = 0;
            
            while (cur_dir[i] != '/')
            {
                cur_dir[i] = 0;
                i--;
            }
        }
    }
}

void execute() {
    command_buffer[command_index] = '\0';
    if(!command_buffer[0]) {
        command_index = 0;
        return;
    }
    u32 argc = 1;
    u32 n = 0;
    while (command_buffer[n])
    {
        if(command_buffer[n] == ' ')
            argc++;
        n++;
    }

    argv_buff = (u32*)malloc(argc * 4);
    argv = (u8**)argv_buff;

    n = 0;
    argc = 1;
    u8* cmd_head = command_buffer;
    argv[0] = cmd_head;
    while (*cmd_head)
    {
        if(*cmd_head == ' ') {
            *cmd_head = '\0';
            argv[argc] = cmd_head + 1;
            argc++;
        }
        cmd_head++;
    }

    u32 is_found = 0;
    for(u32 i = 0;i < COMMAND_COUNT;i++) {
        if(strcmp(argv[0], cmd[i].cmd_name) == 0) {
            cmd[i].handler();
            is_found = 1;
        }
    }

    if(!is_found) {
        u32 err = sys_run(argv[0]);
        if(err) {
            printf("\n%[04NFOS86: Unknown command %s%[07",argv[0]);
        }
    }

    free((void*)argv_buff);

    memset((void*)command_buffer, 0, COMMAND_BUFFER_SIZE);
    command_index = 0;
}

void shell_main() {
    bg_set_color(0);
    fg_set_color(7);
    clear_screen(0);

    printf(info);
    printf("NFOS86$%s> ", cur_dir);
    for(u32 i = 0;i < COMMAND_BUFFER_SIZE;i++) {
        command_buffer[i] = 0;
    }

    command_index = 0;

    u8 c = '\0';
    while (1)
    {
        c = get_char();
        if(c) {
            if(c == ENTER) {
                execute();
                printf("\nNFOS86$%s> ", cur_dir);
            }else if(c == BACKSPACE && command_index != 0) {
                u32 x;
                u32 y;
                get_cursor(&x, &y);
                set_cursor(x - 1, y);
                putchar(' ');
                set_cursor(x - 1, y);
                command_index--;
                command_buffer[command_index] = 0;
            }else {
                if (command_index < COMMAND_BUFFER_SIZE - 1) {
                    if(c != CAPS && c != SHIFT && c != BACKSPACE) {
                        command_buffer[command_index++] = c;
                        putchar(c);
                    }
                }
            }
        }
    }
}

void main() {
    asm("nop");
    cur_dir = malloc(256);
    cur_dir[0] = '/';
    command_buffer = malloc(COMMAND_BUFFER_SIZE);

    shell_main();
}