#ifndef VGA_H
#define VGA_H
#include <lib/int.h>
#include <stdarg.h>

void init_video();

enum vga_color {
    VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

enum graphics_mode {
    VESA_800_600_8,
};

typedef struct VideoDriver{
    //void(*write_string)(const U8* s);
    void(*kputc)(u8 c);
	void(*write_int)(u32 x);
    void(*clear_screen)(u8 color);
	void(*terminal_fg_vbe_set) (u8 color);
	void(*terminal_bg_vbe_set) (u8 color);
	void (*terminal_set_cursor_position)(const u16 x, const u16 y);
	void (*terminal_get_cursor_position)(u16* x, u16* y);
	void (*kprintf)(const u8 *format, ...);
	void (*putpixel)(u32 x, u32 y, u8 color);
}VideoDriver_t;

extern VideoDriver_t* video;

#endif
