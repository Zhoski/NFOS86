#include <drivers/video/video.h>
#include <cpu/paging.h>
#include <drivers/video/vesa_lfb_8bpp.h>

VideoDriver_t* video;

static VideoDriver_t vesa_lfb_8pbb = {
    .kputc = &vesa_lfb_8bpp_draw_char,
    .clear_screen = &vesa_lfb_8bpp_screen_clear,
    .terminal_bg_vbe_set = &vesa_lfb_8bpp_bg_set,
    .terminal_fg_vbe_set = &vesa_lfb_8bpp_fg_set,
    .terminal_set_cursor_position = &vesa_lfb_8bpp_cursor_set_position,
    .terminal_get_cursor_position = &vesa_lfb_8bpp_cursor_get_position,
    .write_int = &vesa_lfb_8bpp_draw_int,
    .kprintf = &vesa_lfb_8bpp_kprintf,
    .putpixel = &vesa_lfb_8bpp_putpixel,
};

void init_video() {
    void* lfb = *(void**)(0x1000 + 64);
    vesa_lfb_8bpp_width = *(u16*)(0x1000 + 68);
    vesa_lfb_8bpp_hight = *(u16*)(0x1000 + 70);
    vesa_lfb_8bpp_pitch = *(u16*)(0x1000 + 74);
    LFBaddr = (u8*)lfb;

    for(u32 i = 0; i < 1024; i++) {
        pg_map_virt_phys(krn_pg_dir, lfb + (i << 12), (void*)(i << 12), 3);
    }

    video = &vesa_lfb_8pbb;
};
