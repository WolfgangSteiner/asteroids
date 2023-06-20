#include "grv/game_state.h"
#include "grv/transform2d.h"
#include "grv/gfxa.h"
#include "grv/math.h"
#include "SDL2/SDL.h"
#include "grv/bitmap_font.h"
#include <string.h>
#include <stdio.h>


void on_draw(game_state_t* game_state) {
    static f32 angle = 0.0f;
    frame_buffer_t* fb = &game_state->frame_buffer;
    vec2i points[] = {{0, 15}, {-5, -10}, {5, -10}};
    const u32 num_points = sizeof(points) / sizeof(vec2i);
    transform2d_t t = {.rotation=angle, .scale=1.0f, .translation={127, 127}};
    transform2d_apply_array(&t, points, num_points);
    frame_buffer_clear(fb);
    
    recti_t a_rect = {4,4,256-8,256-8};
    gfxa_fill_rect_u8(fb, &a_rect, 1);
    gfxa_draw_rect_u8(fb, &a_rect, 4);
    
    gfxa_fill_triangle_u8(fb, points[0], points[1], points[2], 8);
    gfxa_draw_lines_u8(fb, points, num_points, 15, true);
    gfxa_draw_pixel_u8(fb, (vec2i){127,127}, 15);
    angle += 0.5f / 360.0f;
    while (angle > 1.0f) angle -= 1.0f;

    char* text = "Hello World!";
    vec2i text_size = grv_bitmap_font_calc_size(NULL, text);
    vec2i text_pos = { (256 - text_size.x) / 2, 6};
    static f32 phase = 0.0f;
    phase += 0.5f/60.0f;
    u8 color = (u8)(15 * (0.5f * unit_sin_f32(phase) + 0.5f));
    grv_put_text_u8(fb, text, text_pos, NULL, color);
}