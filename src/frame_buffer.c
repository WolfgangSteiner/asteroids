#include "frame_buffer.h"
#include <stdlib.h>

void frame_buffer_init(frame_buffer_t* frame_buffer, frame_buffer_type_t type, u32 width, u32 height) {
    frame_buffer->type = type;
    frame_buffer->width = width;
    frame_buffer->height = height;

    if (type == FRAME_BUFFER_INDEXED) {
        color_palette_init_grayscale(&frame_buffer->palette, 16);
        frame_buffer->data = calloc(width * height, sizeof(u8));
    } else {
        frame_buffer->palette.num_entries = 0;
        frame_buffer->palette.entries = NULL;
        frame_buffer->data = calloc(width * height, sizeof(color_rgba_t));
    }
}


void frame_buffer_clear(frame_buffer_t* frame_buffer) {
    u32 count = frame_buffer->width * frame_buffer->height;

    if (frame_buffer->type == FRAME_BUFFER_INDEXED) {
        u32 count_four = count / 4;
        u32 count_remainder = count % 4;

        u32* dst_four = (u32*) frame_buffer->data;
        for (u32 i = 0; i < count_four; i++) *dst_four++ = 0x0;

        u8* dst_remainder = frame_buffer->data;
        for (u32 i = 0; i < count_remainder; i++) *dst_remainder++ = 0x0;
    } else {
        u32* dst = (u32*)frame_buffer->data;
        for (u32 i = 0; i < count; ++i) *dst++ = 0x0;
    }
}