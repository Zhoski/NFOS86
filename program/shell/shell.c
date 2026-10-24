#include <slib/strio.h>
#include <slib/string.h>
#include <slib/file.h>
#include <slib/memory.h>
#include <slib/types.h>

#define COMMAND_BUFFER_SIZE    128
#define COMMAND_COUNT           11

#define SHIFT 0x01
#define CAPS  0x02
#define CTRL  0x03
#define ENTER 0x0A 
#define BACKSPACE 0x08 

typedef struct __attribute__((packed)) {
    U8 name[8];
    U8 ext[3];
    U16 start_sec;
    U16 size;
    U8 flags;
} _File;

typedef struct Command
{
    U8* cmd_name;
    U32 (*handler)();
};

U32 cmd_help();
U32 cmd_clear();
U32 cmd_dir();
U32 cmd_read();
U32 cmd_write();
U32 cmd_create();
U32 cmd_delete();
U32 cmd_check();
U32 cmd_sys();
U32 cmd_cd();

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
};
U32 command_index = 0;
U8 *command_buffer;

U32 *argv_buff;
U8** argv;

U8* info = "Copyright (c) 2026 Zhoski. Licensed under the MIT License.\n"
           "Type 'help' to get a list of commands.\n\n";

U8* cur_dir;

U32 cmd_help() {
    U8* help_msg =   "\n"
                        "[BASE]\n"
                        "   clear                -- clear screen\n"
                        "[DISK]\n"
                        "   touch  [file]        -- creates file\n"
                        "   rm     [file]        -- deletes file\n"
                        "   cat    [file]        -- read file\n"
                        "   write  [file] [data] -- write in file\n"
                        "   rmdir  [dir]         -- deletes dir\n"
                        "   mkdir  [dir]         -- creates dir\n"
                        "   ls                   -- displays all files in a directory\n"
                        "[SYSTEM]\n"
                        "   sys                  -- show system info\n"
                        "   setdrv [type] [file] -- change driver\n"
                        "   getdrv               -- show list cur drv";
    printf(help_msg);
}

U32 cmd_clear() {
    clear_screen(0);

    printf(info);
}

/**
 *  FIX: Исправить на fopen в будущем
 */
U32 cmd_read() {
    putchar('\n');
    /*File* f = fopen(argv[1], FREAD);
    if(!f) {
        printf("%[12Can't open [%s]%[15",argv[1]);
        return;
    }
    if(!f->cur) {
        printf("[File empty]");
    }else {
        U8* buffer = malloc(f->bytes);
        fread(f, f->bytes, buffer);
        buffer[f->bytes] = '\0';
        printf("%s",buffer);
    }
    fclose(f);*/

    U8* full_path = malloc(strlen(cur_dir) + strlen(argv[1]) + 1);
    strcat(full_path, cur_dir + 1);
    strcat(full_path, argv[1]);

    uint8* temp_buffer = malloc(4096);
    memset(temp_buffer, 0, 4096);
    U32 read_status = sys_read(full_path, 4096, temp_buffer);

    if(read_status == 0) {
        printf("%s", temp_buffer);
    }else {
        printf("[ %[04FAIL%[07 ] File %s not found", argv[1]);
    }
    free(temp_buffer);

    free(full_path);
}

U32 cmd_check() {
    putchar('\n');
    U8* full_path = malloc(strlen(cur_dir) + strlen(argv[1]) + 1);
    strcat(full_path, cur_dir + 1);
    strcat(full_path, argv[1]);

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
U32 cmd_write() {
    putchar('\n');
    /*int size = 0;
    File* f = fopen(argv[1], FWRITE);
    if(!f) {
        printf("%[12Can't write [%s]%[15",argv[1]);
        return;
    }else {
        U32 s = 0;
        while (argv[2][s++]);
        
        //sys_write(f->name, argv[2], s);
        fwrite(f, s, argv[2]);
    }
    fclose(f);*/
    U8* full_path = malloc(strlen(cur_dir) + strlen(argv[1]) + 1);
    strcat(full_path, cur_dir + 1);
    strcat(full_path, argv[1]);

    if(sys_write(full_path, argv[2], strlen(argv[2])) == 0x00) {
        printf("[  %[02OK%[07  ] File %s overwrite", argv[1]);
    }else {
        printf("[ %[04FAIL%[07 ] File %s not found", argv[1]);
    }

    free(full_path);
}

U32 cmd_delete() {
    putchar('\n');
    U8* full_path = malloc(strlen(cur_dir) + strlen(argv[1]) + 1);
    strcat(full_path, cur_dir + 1);
    strcat(full_path, argv[1]);

    int ret = sys_delete(full_path);
    if(!ret == 0) {
        printf("[ %[04FAIL%[07 ] File %s not found", argv[1]);
    }else {
        printf("[  %[02OK%[07  ] File %s succes deleted", argv[1]);
    }

    free(full_path);
}

U32 cmd_create() {
    putchar('\n');
    U8* full_path = malloc(strlen(cur_dir) + strlen(argv[1]) + 1);
    strcat(full_path, cur_dir + 1);
    strcat(full_path, argv[1]);

    if(sys_create(argv[1], 512) == 0) {
        printf("[  %[02OK%[07  ] File %s succes created", argv[1]);
    } else {
        printf("\n[ %[04FAIL%[07 ] File %s create error", argv[1]);
    }

    free(full_path);
}

U32 cmd_dir() {
    printf("\n");
    U8* full_path = malloc(strlen(cur_dir) + strlen(argv[1]) + 1);
    strcat(full_path, cur_dir + 1);
    strcat(full_path, argv[1]);

    write_root(full_path);

    command_index = 0;
    memset(&command_buffer, 0, COMMAND_BUFFER_SIZE);

    free(full_path);
}

U32 cmd_sys() {
    printf("\n");
    U16* ram = (U16*)0x1000;
    U8*  cpu = malloc(48);
    memcpy(0x1008, cpu, 48);

    printf("\n");
    printf("OS: NFOS86 Demo\n"); 
    printf("Kernel: v0.0.1-custom\n");
    printf("CPU: %s\n", cpu); 
    printf("Memory: %dk\n", *ram); 
    printf("Display: VGA 640x480 16 colors\n"); 
    printf("Shell: Default\n");
    printf("FS: FluidFS\n");
    printf("\n%[01\xDB\xDB%[02\xDB\xDB%[03\xDB\xDB%[04\xDB\xDB%[05\xDB\xDB"
        "%[06\xDB\xDB%[07\xDB\xDB%[08\xDB\xDB%[09\xDB\xDB%[10\xDB\xDB%[11\xDB\xDB"
        "%[12\xDB\xDB%[13\xDB\xDB%[14\xDB\xDB%[15\xDB\xDB%[07\n");
}

U32 cmd_cd() {
    printf("\n");
    if(sys_check(argv[1])) {
        //U8* tmp = malloc(strlen(cur_dir));
        //free(cur_dir);

        //cur_dir = malloc(strlen(tmp) + strlen(argv[1]) + 2);
        strcat(cur_dir, argv[1]);
        strcat(cur_dir, "/");

        //free(tmp);

        printf("[%[02  OK  %[07] Directory changed");
    }else {
        printf("[%[04 FAIL %[07] Directory not found");
    }
}

void execute() {
    command_buffer[command_index] = '\0';
    if(!command_buffer[0]) {
        command_index = 0;
        return;
    }
    U32 argc = 1;
    U32 n = 0;
    while (command_buffer[n])
    {
        if(command_buffer[n] == ' ')
            argc++;
        n++;
    }

    argv_buff = (U32)malloc(argc * 4);
    argv = argv_buff;

    n = 0;
    argc = 1;
    U8* cmd_head = command_buffer;
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

    U32 is_found = 0;
    for(U32 i = 0;i < COMMAND_COUNT;i++) {
        if(strcmp(argv[0], cmd[i].cmd_name) == 0) {
            cmd[i].handler();
            is_found = 1;
        }
    }

    memset(&command_buffer, 0, COMMAND_BUFFER_SIZE);

    if(!is_found) {
        U32 err = sys_run(argv[0]);
        if(err) {
            printf("\n%[04NFOS86: Unknown command %s%[07",argv[0]);
        }
    }

    free(argv_buff);

    command_index = 0;
}

void shell_main() {
    SetBGColor(0);
    SetFGColor(7);
    clear_screen(0);

    printf(info);
    printf("NFOS86$%s> ", cur_dir);
    for(U32 i = 0;i < COMMAND_BUFFER_SIZE;i++) {
        command_buffer[i] = 0;
    }

    command_index = 0;

    U8 c = '\0';
    while (1)
    {
        c = get_char();
        if(c) {
            if(c == ENTER) {
                execute();
                printf("\nNFOS86$%s> ", cur_dir);
            }else if(c == BACKSPACE && command_index != 0) {
                U32 x;
                U32 y;
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
    cur_dir = malloc(256);
    cur_dir[0] = '/';
    command_buffer = malloc(COMMAND_BUFFER_SIZE);

    shell_main();
}