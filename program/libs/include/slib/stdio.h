#ifndef STRIO_H
#define STRIO_H

void fg_set_color(unsigned char color);
void bg_set_color(unsigned char color);
void printf(const unsigned char *format, ...);
void putchar(unsigned char c);
void clear_screen(unsigned char color);
void set_cursor(const unsigned short x, const unsigned short y);
void get_cursor(unsigned int* x, unsigned int* y);

unsigned char get_char();

#endif