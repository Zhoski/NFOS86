#include <drivers/video/vesa_lfb_8bpp.h>
#include <kernel/memory.h>
#include <lib/debug.h>

#define FONT8x16_IMPLEMENTATION
#include "../../font/font8x16.h"

u32 screen_x_off = 0;
u32 screen_y_off = 0;

u8 terminal_fg_vbe = 15;
u8 terminal_bg_vbe = 0;

u8* LFBaddr;
u16 vesa_lfb_8bpp_width;
u16 vesa_lfb_8bpp_hight;
u16 vesa_lfb_8bpp_pitch;

void vesa_lfb_8bpp_fg_opcode(u32 code) {
    vesa_lfb_8bpp_fg_set(code);
}

void vesa_lfb_8bpp_bg_opcode(u32 code) {
    vesa_lfb_8bpp_bg_set(code);
}

/**
 *  Очищает экран в цвет color
 */
void vesa_lfb_8bpp_screen_clear(u8 color) {
    screen_x_off = 0;
    screen_y_off = 0;

    memset(LFBaddr, color, vesa_lfb_8bpp_width * vesa_lfb_8bpp_hight);
}

void vesa_lfb_8bpp_write_row(u32 x, u32 y, u8 b, u8 fg, u8 bg) {
    u32 pixel_offset = y * vesa_lfb_8bpp_pitch + x;
    u32 j = 7;
    for(u32 i = 0; i < 8; i++) {
        if(b & (1 << j)) LFBaddr[pixel_offset + i] = fg;
        else LFBaddr[pixel_offset + i] = bg;
        j--;
    }
}

/**
 *  Закрашивает пиксель (x, y) в цвет color
 */
void vesa_lfb_8bpp_putpixel(u32 x, u32 y, u8 color) { 
    u32 pixel_offset = y * vesa_lfb_8bpp_pitch + (x * (8/8));
    LFBaddr[pixel_offset] = color;
}

/**
 *  Прокручивает экран вниз на один символ
 */
void vesa_lfb_8bpp_scroll() {
    //memcpy((vesa_lfb_8bpp_MEMORY + 80 * 16), (vesa_lfb_8bpp_MEMORY), 80 * 480);
    memcpy(LFBaddr + vesa_lfb_8bpp_pitch * 16, LFBaddr, vesa_lfb_8bpp_width * vesa_lfb_8bpp_hight);

    //service.memory->memset((vesa_lfb_8bpp_MEMORY + 80), terminal_bg_vbe, 80);
}

/**
 *  Выводит символ, прокручивает экран
 */
void vesa_lfb_8bpp_draw_char(u8 c) {
    if(c == '\n' || screen_x_off == vesa_lfb_8bpp_width) {
        screen_x_off = 0;
        screen_y_off += 16;
        if(screen_y_off >= vesa_lfb_8bpp_hight - 16) {
            vesa_lfb_8bpp_scroll();
            screen_y_off -= 16;
        }
        return;
    }
    for(u32 row = 0; row < 16;row++) {
        u8 row_byte = font8x16[c][row];
        vesa_lfb_8bpp_write_row(screen_x_off, screen_y_off + row, row_byte, terminal_fg_vbe, terminal_bg_vbe);
    }

    screen_x_off+=8;
}

void vesa_lfb_8bpp_draw_string(const u8* s) {
    while(*s) {
        vesa_lfb_8bpp_draw_char(*s);
        s++; 
    }
}

void vesa_lfb_8bpp_draw_int(u32 x) {
    u32 i = 0;
    u32 isNegative = 0;

    u8 buffer[10];

    if (x < 0) {
        isNegative = 1;
        x = -x;
    }

    do {
        buffer[i++] = (x % 10) + '0';
        x /= 10;
    } while (x > 0);

    if (isNegative) {
        buffer[i++] = '-';
    }

    buffer[i] = '\0';

    u32 start = 0;
    u32 end = i - 1;
    while (start < end) {
        u8 temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;
        start++;
        end--;
    }

    vesa_lfb_8bpp_draw_string(buffer);
}

void vesa_lfb_8bpp_cursor_set_position(const u16 x, const u16 y) {
    screen_x_off = x << 3;
    screen_y_off = y << 4;
}

void vesa_lfb_8bpp_cursor_get_position(u16* x, u16* y)  {
    *x = screen_x_off >> 3;
    *y = screen_y_off >> 4;
}

void vesa_lfb_8bpp_fg_set(u8 color) {
    terminal_fg_vbe = color;
}

void vesa_lfb_8bpp_bg_set(u8 color) {
    terminal_bg_vbe = color;
}

/**
 *  Форматированный вывод бесконечного числа аргументов
 */
void vesa_lfb_8bpp_kprintf(const u8 *format, ...) {
    va_list args;
    va_start(args, format);

    for(const i8 *p = format; *p != '\0'; p++) {
        if(*p != '%') {
            vesa_lfb_8bpp_draw_char(*p);
            continue;
        }

        p++;

        switch (*p)
        {
        case 'd': {
            i32 n = va_arg(args, i32);
            i32 i = 0;
            i32 isNegative = 0;

            i8 buffer[10] = {0};

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

            i32 start = 0;
            i32 end = i - 1;
            while (start < end) {
                i8 temp = buffer[start];
                buffer[start] = buffer[end];
                buffer[end] = temp;
                start++;
                end--;
            }
            
            vesa_lfb_8bpp_draw_string(buffer);

            break;
        }
        case 'x': {
            u32 n = va_arg(args, u32);
            u32 i = 0;

            u8 buffer[10] = {0};

            do {
                u8* chars = "0123456789ABCDEF";
                u32 hex = n % 16;
                buffer[i++] = chars[hex];
                n /= 16;
            } while (n > 0);

            buffer[i] = '\0';

            u32 start = 0;
            u32 end = i - 1;
            while (start < end) {
                u8 temp = buffer[start];
                buffer[start] = buffer[end];
                buffer[end] = temp;
                start++;
                end--;
            }
            
            vesa_lfb_8bpp_draw_string(buffer);
            break;
        }
        case 's':
            u8 *s = va_arg(args, u8*);
            vesa_lfb_8bpp_draw_string(s);
            break;
        case 'c':
            u8 *c = va_arg(args, u8*);
            vesa_lfb_8bpp_draw_char(*c);
            break;
        case 'f': {
            const u8 code[2] = {*(p+1), *(p+2)};
            u32 result = 0;
            u8 c = code[0];
            result = result * 10 + (c - '0');
            c = code[1];
            result = result * 10 + (c - '0');
            vesa_lfb_8bpp_fg_opcode(result);
            p += 2;

            break;
        }
        case 'b': {
            const u8 code[2] = {*(p+1), *(p+2)};
            u32 result = 0;
            u8 c = code[0];
            result = result * 10 + (c - '0');
            c = code[1];
            result = result * 10 + (c - '0');
            vesa_lfb_8bpp_bg_opcode(result);
            p += 2;

            break;
        }
        default:
            break;
        }
    }
}