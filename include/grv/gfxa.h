#ifndef GFXA_H
#define GFXA_H

#include "grv/vec2i.h"
#include "grv/recti.h"
#include "grv/frame_buffer.h"

void gfxa_draw_pixel_u8(frame_buffer_t* fb, vec2i p, u8 color);
void gfxa_draw_pixels_u8(frame_buffer_t* fb, vec2i* points, u32 count, u8 color);

void gfxa_draw_line_u8(frame_buffer_t* fb, vec2i p1, vec2i p2, u8 color);
void gfxa_draw_lines_u8(frame_buffer_t* fb, vec2i* points, u32 count, u8 color, bool close_shape);

void gfxa_fill_rect_u8(frame_buffer_t* fb, recti_t* r, u8 color);
void gfxa_draw_rect_u8(frame_buffer_t* fb, recti_t* r, u8 color);

void gfxa_fill_horizontal_span_u8(frame_buffer_t* fb, s32 y, s32 x1, s32 x2, u8 color);

void gfxa_fill_triangle_u8(frame_buffer_t* fb, vec2i p1, vec2i p2, vec2i p3, u8 color); 

// the caller is responsible to ensure that at least part of the span lies within the fb
void gfxa_fill_horizontal_span_clipped_u8(frame_buffer_t* fb, s32 y, s32 x1, s32 x2, u8 color);

// the caller is responsible to ensure that at least part of the span lies within the fb
void gfxa_fill_vertical_span_u8(frame_buffer_t* fb, s32 y1, s32 y2, s32 x, u8 color);

void gfxa_scan_convert_triangle(frame_buffer_t* fb, vec2i p1, vec2i p2, vec2i p3);
void gfxa_fill_spans_clipped_u8(frame_buffer_t* fb, u8 color);
void gfxa_fill_spans_wrapped_u8(frame_buffer_t* fb, u8 color);
#endif