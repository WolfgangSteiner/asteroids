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