#include <slib/string.h>

int strlen(const char s[]) {
    unsigned int i = 0;
    while(s[i] != '\0')
        i++;

    return i;
}

int strcmp(const char *s1, const char *s2) {
    while(*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

void itos(int num, char* out) {
    char *p = out;

    if (num == 0) {
        return;
    }

    while (num > 0) {
        *p++ = (char)((num % 10) + '0');
        num /= 10;
    }
}

void strcat(char* dest, char* src) {
    int i = 0, j = 0;

    while (dest[i]) {
        i++;
    }
    while (src[j])
    {
        dest[i] = src[j];
        i++;
        j++;
    }
    
}

void strcpy(char* dest, char* src) {
    asm volatile (
        "movl %[src], %%esi\n"
        "movl %[dest],   %%edi\n"
        "movl %[count],%%ecx\n"
        "rep movsb"
        :
        : [dest] "r" ((unsigned char*)dest), [src] "r" ((unsigned char*)src), [count] "r" (strlen(src))
        : "esi", "edi", "ecx","memory"
    ); 
}