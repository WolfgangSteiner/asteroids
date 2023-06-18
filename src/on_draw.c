#include "grv/game_state.h"
#include "grv/transform2d.h"
#include "grv/gfxa.h"
#include "SDL2/SDL.h"


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
}