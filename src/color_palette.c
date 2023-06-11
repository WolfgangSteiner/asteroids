#include "color_palette.h"
#include <stdlib.h>

void color_palette_init(color_palette_t* palette, u32 num_entries) {
    palette->num_entries = num_entries;
    palette->entries = calloc(num_entries, sizeof(color_rgba_t));
}

void color_palette_init_grayscale(color_palette_t* palette, u32 num_entries) {
    color_palette_init(palette, num_entries);
    for(u32 i = 0; i < num_entries; i++) {
        u8 value = (u8)((f32)i / (f32)(num_entries - 1) * 255.0f);
        palette->entries[i].r = value;
        palette->entries[i].g = value;
        palette->entries[i].b = value;
        palette->entries[i].a = 255;
    }
}

color_rgba_t color_palette_map(color_palette_t* palette, u8 index) {
    return palette->entries[index % palette->num_entries];
}