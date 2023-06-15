#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include "base_defines.h"
#include "color_palette.h"

typedef enum {
    FRAME_BUFFER_INDEXED,
    FRAME_BUFFER_RGBA,
} frame_buffer_type_t;

typedef struct {
    frame_buffer_type_t type;
    u32 width, height;
    color_palette_t palette;
    u8* data;
} frame_buffer_t;

void frame_buffer_init(frame_buffer_t* frame_buffer, frame_buffer_type_t type, u32 width, u32 height);
void frame_buffer_clear(frame_buffer_t* frame_buffer);
#endif