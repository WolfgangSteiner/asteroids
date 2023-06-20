#ifndef GRV_BITMAP_FONT_H
#define GRV_BITMAP_FONT_H

#include "grv/frame_buffer.h"
#include "grv/vec2i.h"

typedef struct bitmap_font_t {
    u8* glyph_data;
    u32 glyph_count;
    u8 glyph_width;
    u8 glyph_height;
    s32 hskip;
    s32 vskip;
} bitmap_font_t;

void grv_put_text_u8(frame_buffer_t* fb, const char* text, vec2i pos, bitmap_font_t* font, u8 color);
vec2i grv_bitmap_font_calc_size(bitmap_font_t* font, char* text);

#endif