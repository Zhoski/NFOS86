#ifndef __MEMORY__
#define __MEMORY__

void memset(void* in, unsigned char v, unsigned int n);
void memcpy(void* src,void* dest, unsigned int n);
unsigned char* malloc(unsigned int n);
void free(void* ptr);

#endif