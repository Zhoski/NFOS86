#ifndef __MEMORY__
#define __MEMORY__

void memset(void* in, unsigned char v, unsigned int n);
void memcpy(void* src,void* dest, unsigned int n);
void* malloc(unsigned int n);
void free(void* ptr);
unsigned int memcmp(void* p1, void* p2, unsigned int n);
#endif