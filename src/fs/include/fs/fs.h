#ifndef FS
#define FS
#include <lib/int.h>
#include <fs/fluidfs.h>

#define AFS_T       0x01
#define FLUIDFS_T   0x02

typedef struct FileSystem
{
    u32 (*check)(u8* file_name);
    u32 (*open)(u8* file_name);
    u32 (*read)(u8* file_name, u8* dest, u32 n);
    u32 (*create)(u8* file_name, u8 flag);
    u32 (*delete)(u8* file_name);
    u32 (*update) (u8* file_name, u8* src, u32 n);
    u32 (*write_dir) (u8 * dest);
    u32 (*init)();
} FileSystem_t;

u32 init_fs(u32 type);

extern FileSystem_t* fs;
extern node_t g_node;

#endif
