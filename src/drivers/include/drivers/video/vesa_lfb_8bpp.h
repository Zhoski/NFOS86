#ifndef __vesa_lfb_8bpp__
#define __vesa_lfb_8bpp__
#include <lib/int.h>
#include <stdarg.h>

void vesa_lfb_8bpp_screen_clear(U8 color);
void vesa_lfb_8bpp_putpixel(U32 x, U32 y, U8 color);
void vesa_lfb_8bpp_draw_char(U8 c);
void vesa_lfb_8bpp_draw_string(const U8* s);
void vesa_lfb_8bpp_draw_int(U32 x);
void vesa_lfb_8bpp_fg_set(U8 color);
void vesa_lfb_8bpp_bg_set(U8 color);
void vesa_lfb_8bpp_cursor_set_position(const U16 x, const U16 y);
void vesa_lfb_8bpp_cursor_get_position(U16* x, U16* y);
void vesa_lfb_8bpp_kprintf(const u8 *format, ...);

extern u8* LFBaddr;
extern u16 vesa_lfb_8bpp_width;
extern u16 vesa_lfb_8bpp_hight;
extern u16 vesa_lfb_8bpp_pitch;

#endif