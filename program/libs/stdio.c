#include <slib/stdio.h>
#include <stdarg.h>

void sequence_black() {
    fg_set_color(0);
}

void sequence_blue() {
    fg_set_color(1);
}

void sequence_green() {
    fg_set_color(2);
}


void sequence_cyan() {
    fg_set_color(3);
}

void sequence_red() {
    fg_set_color(4);
}

void sequence_magenta() {
    fg_set_color(5);
}

void sequence_brown() {
    fg_set_color(6);
}

void sequence_light_grey() {
    fg_set_color(7);
}

void sequence_dark_grey() {
    fg_set_color(8);
}

void sequence_light_blue() {
    fg_set_color(9);
}

void sequence_light_green() {
    fg_set_color(10);
}

void sequence_light_cyan() {
    fg_set_color(11);
}

void sequence_light_red() {
    fg_set_color(12);
}

void sequence_light_magenta() {
    fg_set_color(13);
}

void sequence_light_brown() {
    fg_set_color(14);
}

void sequence_white() {
    fg_set_color(15);
}

typedef struct {
    unsigned char* sequence;
    void (*handler)();
}control_sequence;

const control_sequence vga_colors[] = {
    {.sequence = "00", .handler = sequence_black},
    {.sequence = "01", .handler = sequence_blue},
    {.sequence = "02", .handler = sequence_green},
    {.sequence = "03", .handler = sequence_cyan},
    {.sequence = "04", .handler = sequence_red},
    {.sequence = "05", .handler = sequence_magenta},
    {.sequence = "06", .handler = sequence_brown},
    {.sequence = "07", .handler = sequence_light_grey},
    {.sequence = "08", .handler = sequence_dark_grey},
    {.sequence = "09", .handler = sequence_light_blue},
    {.sequence = "10", .handler = sequence_light_green},
    {.sequence = "11", .handler = sequence_light_cyan},
    {.sequence = "12", .handler = sequence_light_red},
    {.sequence = "13", .handler = sequence_light_magenta},
    {.sequence = "14", .handler = sequence_light_brown},
    {.sequence = "15", .handler = sequence_white}
};

int __strcmp(const char *s1, const char *s2) {
    while(*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

int __strlen(const char s[]) {
    unsigned int i = 0;
    while(s[i] != '\0')
        i++;

    return i;
}

void fg_set_color(unsigned char color) {
    asm volatile(
        "movl $1, %%eax\n"
        "movl $6, %%ebx\n"
        "movl %0, %%ecx\n"
        "int $0x40"
        :
        : "r"((unsigned int)color)
        : "eax", "ebx", "ecx", "memory"
    );
}

void bg_set_color(unsigned char color) {
    asm volatile(
        "movl $1, %%eax\n"
        "movl $5, %%ebx\n"
        "movl %0, %%ecx\n"
        "int $0x40"
        :
        : "r"((unsigned int)color)
        : "eax", "ebx", "ecx", "memory"
    );
}

void printf(const unsigned char *format, ...) {
    va_list args;
    va_start(args, format);

    for(const char *p = format; *p != '\0'; p++) {
        if(*p != '%') {
            asm volatile (
                "movl $1, %%eax\n"
                "movl $2, %%ebx\n"
                "movl %0, %%ecx\n"
                "int $0x40"
                :
                : "r" ((unsigned int)*p)
                : "eax", "ebx","ecx", "memory"
            );
            continue;
        }

        p++;

        switch (*p)
        {
        case 'd':
            int n = va_arg(args, int);
            int i = 0;
            int isNegative = 0;

            char buffer[10];

            if (n < 0) {
                isNegative = 1;
                n = -n;
            }

            do {
                buffer[i++] = (n % 10) + '0';
                n /= 10;
            } while (n > 0);

            if (isNegative) {
                buffer[i++] = '-';
            }

            buffer[i] = '\0';

            int start = 0;
            int end = i - 1;
            while (start < end) {
                char temp = buffer[start];
                buffer[start] = buffer[end];
                buffer[end] = temp;
                start++;
                end--;
            }
            
            asm volatile (
                "movl $1, %%eax\n"
                "movl $1, %%ebx\n"
                "movl %0, %%ecx\n"
                "int $0x40"
                :
                : "r" (buffer)
                : "eax", "ebx","ecx", "memory"
            );

            break;
        case 's':
            char *s = va_arg(args, char*);
            asm volatile (
                "movl $1, %%eax\n"
                "movl $1, %%ebx\n"
                "movl %0, %%ecx\n"
                "int $0x40"
                :
                : "r" (s)
                : "eax", "ebx","ecx", "memory"
            );
            break;
        case '[':
            const char code[3] = {*(p+1), *(p+2), 0};  // Следущие два символа
            for(int i = 0;i < 16;i++) {
                if(__strcmp(code, vga_colors[i].sequence) == 0) {
                    vga_colors[i].handler();
                    break;
                }
            }
            p += 2;
            break;
        default:
            break;
        }
    }
}

void putchar(unsigned char c) {
    asm volatile (
        "movl $1, %%eax\n"
        "movl $2, %%ebx\n"
        "movl %0, %%ecx\n"
        "int $0x40"
        :
        : "r" ((unsigned int)c)
        : "eax", "ebx","ecx", "memory"
    );
}

void clear_screen(unsigned char color) {
    asm volatile (
        "movl $1, %%eax\n"
        "movl $7, %%ebx\n"
        "movl %0, %%edx\n"
        "int $0x40"
        :
        : "r" ((unsigned int)color)
        : "eax", "ebx", "edx", "memory"
    );
}

void set_cursor(const unsigned short x, const unsigned short y) {
    unsigned int pos = x;
    pos <<= 16;
    pos |= y;
    
    asm volatile (
        "movl $1, %%eax\n"
        "movl $8, %%ebx\n"
        "movl %0, %%ecx\n"
        "int $0x40"
        :
        : "r" (pos)
        : "eax", "ebx", "ecx", "memory"
    );
}

void get_cursor(unsigned int* x, unsigned int* y) { 
    asm volatile (
        "movl $1, %%eax\n"
        "movl $9, %%ebx\n"
        "movl %0, %%ecx\n"
        "movl %1, %%edx\n"
        "int $0x40"
        :
        : "r" (x), "r" (y)
        : "eax", "ebx", "ecx", "edx", "memory"
    );
}

unsigned char get_char() {
    unsigned char out; 
    asm volatile (
        "movl $2, %%eax\n"
        "movl $1, %%ebx\n"
        "movl $0, %%ecx\n"
        "movl $0, %%edx\n"
        "int $0x40"
        : "=a"(out) 
        : 
        : "ebx", "ecx", "edx", "memory"
    );
    return out;
}
