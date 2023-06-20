#include "grv/bitmap_font.h"
#include "grv/gfxa.h"
#include "grv/math.h"
#include <string.h>
#include "basic_font_8x8_impl.c"

static void grv_put_char_u8(frame_buffer_t* fb, char c, vec2i pos, bitmap_font_t* font, u8 color) {
    u8* glyph_data = font->glyph_data + font->glyph_height * (u32)c;
    for (int y = 0; y < font->glyph_height; y++) {
        u8 glyph_row = *glyph_data++;
        for (int x = 0; x < 8; ++x) {
            bool has_pixel = (glyph_row >> x) & 0x1;
            if (has_pixel) {
                vec2i pixel_pos = vec2i_add(pos, (vec2i){x, y});
                gfxa_draw_pixel_u8(fb, pixel_pos, color);
            }
        }
    }
}

void grv_put_text_u8(frame_buffer_t* fb, const char* text, vec2i pos, bitmap_font_t* font, u8 color) {
    vec2i line_pos = pos;
    vec2i glyph_pos = pos;
    if (font == NULL) font = &basic_font_8x8;

    char c;
    while((c = *text++) != '\0') {
        if (c == '\n') {
            line_pos.y += font->vskip;
            glyph_pos = line_pos;
            continue;
        } else if (c == ' ') {
            glyph_pos.x += font->hskip;
            continue;
        }
        grv_put_char_u8(fb, c, glyph_pos, font, color);
        glyph_pos.x += font->hskip;
    }
}

vec2i grv_bitmap_font_calc_size(bitmap_font_t* font, char* text) {
    if (font == NULL) font = &basic_font_8x8;
    s32 x = 0;
    vec2i size = {0};
    size_t len = strlen(text);
    if (len == 0) return size;
    size.y = font->glyph_height;

    char c;
    while((c = *text++) != '\0') {
        if (c == '\n') {
            size.y += font->vskip;
            x = 0;
        } else { 
            x += font->hskip;
            size.x = max_s32(size.x, x);
        }
    }
    return size;
}