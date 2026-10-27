#include <kernel/pmm.h>
#include <drivers/video/video.h>

u8 pg_bitmap[512];      // 16Мб максимум
u32 pg_c = 0;
u32 pg_addr = 0;

/**
 *  Заполняет облась памяти начиная с addr и размером len 4КиБ страницами
 */
void pg_mk_alloc(u32 addr, u32 len) {
    pg_c = len >> 12;
    pg_addr = addr;
    
    for(u32 i = 0; i < 512; i++) {
        pg_bitmap[i] = 0;
    }
}

/**
 *  Возвращает адрес свободной страницы
 */
void* pg_alloc() {
    u32 byte = 0;
    u32 bit_in_byte = 0;
    for(u32 i = 0; i < pg_c; i++) {
        byte = i / 8;
        bit_in_byte = i % 8;

        if((pg_bitmap[byte] & (1 << bit_in_byte)) == 0) {
            pg_bitmap[byte] |= (1 << bit_in_byte);
            return (void*)(pg_addr + (i << 12));
        }
    }

    return 0;
}

/**
 *  Сбрасывает бит соответсвующий pg тем самым освобождая страницу
 */
void pg_free(void* pg) {
    u32 bit_in_bitmap = ((u32)pg - pg_addr) >> 12;
    u32 byte = bit_in_bitmap / 8;
    u32 bit = bit_in_bitmap % 8;

    pg_bitmap[byte] &= ~(1 << bit);
}