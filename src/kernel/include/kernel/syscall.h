#ifndef __SYSCALL__
#define __SYSCALL__

#include <lib/int.h>

#define SYS_EXIT            0
#define WRITE               1
#define SCREEN              0
#define COM_PORT            1
#define STR                 0
#define SYM                 1
#define HEX                 2
#define KEYBOARD            2
#define NO_WAIT             0
#define WAIT                1
#define MALLOC              3
#define FREE                4
#define SYS_OPEN            5
#define SYS_CLOSE           6
#define SYS_CREATE          7
#define SYS_DELETE          8
#define SYS_RUN             9
#define SET_BG              10
#define SET_FG              11
#define CLEAR               12
#define SET_CURSOR          13
#define GET_CURSOR          14
#define SYS_WRITE           15
#define WRITE_DIR           16
#define INIT_HEAP           17

typedef struct {
    u32 eax; u32 ebx; u32 ecx; u32 edx; u32 esi; u32 edi; u32 gs; u32 fs; u32 es; u32 ds;
} __attribute__((packed)) syscall_ct;

void syscall_handler(syscall_ct* ct);
#endif
