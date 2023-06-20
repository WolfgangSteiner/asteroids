#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include "grv/common.h"
#include "grv/color_palette.h"
#include "grv/recti.h"

typedef enum {
    FRAME_BUFFER_INDEXED,
    FRAME_BUFFER_RGBA,
} frame_buffer_type_t;

typedef struct {
    s32 x1, x2;
} grv_span_t;

typedef struct {
    grv_span_t* spans;
    u32 capacity;
    u32 count;
    s32 y_start;
} grv_span_buffer_t;

typedef struct {
    recti_t* stack;
    u32 capacity;
    u32 size;
} grv_clipping_stack_t;

void grv_clipping_stack_init(grv_clipping_stack_t* stack);
void grv_clipping_stack_push(grv_clipping_stack_t* stack, recti_t rect);
recti_t grv_clipping_stack_pop(grv_clipping_stack_t* stack);
recti_t* grv_clipping_stack_top(grv_clipping_stack_t* stack);

typedef struct {
    frame_buffer_type_t type;
    u32 width, height;
    color_palette_t palette;
    u8* data;
    bool use_clipping;
    grv_clipping_stack_t clipping_stack;
    grv_span_buffer_t span_buffer;
} frame_buffer_t;

void frame_buffer_init(frame_buffer_t* frame_buffer, frame_buffer_type_t type, u32 width, u32 height);
void frame_buffer_clear(frame_buffer_t* frame_buffer);
recti_t frame_buffer_get_clipping_rect(frame_buffer_t* fb);
u8* frame_buffer_pixel_address_u8(frame_buffer_t* fb, s32 x, s32 y);
void frame_buffer_clear_span_buffer(frame_buffer_t* fb);
void frame_buffer_push_span(frame_buffer_t* fb, s32 y, s32 x_start, s32 x_end);
#endif