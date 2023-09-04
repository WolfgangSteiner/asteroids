#ifndef COLOR_PALETTE_H
#define COLOR_PALETTE_H

#include "grv/grv_base.h"
#include "grv/color_rgba.h"

typedef struct {
    u32 num_entries;
    color_rgba_t* entries;
} color_palette_t;

void color_palette_init(color_palette_t* palette, u32 num_entries);
void color_palette_init_grayscale(color_palette_t* palette, u32 num_entries);
color_rgba_t color_palette_map(color_palette_t* palette, u8 index);
#endif