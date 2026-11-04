#ifndef FILE
#define FILE

#define FWRITE      0x00
#define FREAD       0x01

#define BASE        0x02
#define END         0x03

unsigned int sys_check(const char* f);
unsigned int sys_read(const char* f, unsigned int n, unsigned char* dest);
unsigned int sys_write(const char* f, unsigned char* src, unsigned int n);
unsigned int sys_create(const char* f, unsigned char flag);
unsigned int sys_delete(const char* f);
unsigned int sys_run(const char* f);
unsigned int sys_died();
unsigned int sys_open(const char* f);
unsigned int write_root(const char* dir);
void sys_close(unsigned int fd);

#endif
