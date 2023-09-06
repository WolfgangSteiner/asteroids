#include "grv/gfxa.h"
#include "grv/recti.h"
#include "grv/vec2f.h"
#include "grv/grv_math.h"
#include "grv/grv_common.h"
#include <math.h>
#include <assert.h>
#include <string.h>

void gfxa_draw_pixel_u8(frame_buffer_t* fb, vec2i p, u8 color) {
    if (p.x >= 0 && p.x < (s32)fb->width && p.y >= 0 && p.y < (s32)fb->height) {
        fb->data[p.y * fb->width + p.x] = color;
    }
}


void gfxa_draw_pixels_u8(frame_buffer_t* fb, vec2i* points, u32 count, u8 color) {
    for (u32 i = 0; i < count; i++) {
        gfxa_draw_pixel_u8(fb, points[i], color);
    }
}

typedef enum {
    CLIP_OUTCODE_INSIDE = 0x0,
    CLIP_OUTCODE_LEFT = 0x1,
    CLIP_OUTCODE_RIGHT = 0x2,
    CLIP_OUTCODE_BOTTOM = 0x4,
    CLIP_OUTCODE_TOP = 0x8
} clip_outcode_t;

static u32 compute_outcode(vec2f p, recti_t cr) {
    u32 code = CLIP_OUTCODE_INSIDE;
    if (p.x  < cr.x) code |= CLIP_OUTCODE_LEFT;
    else if (p.x > cr.x + cr.w - 1) code |= CLIP_OUTCODE_RIGHT;
    if (p.y < cr.y) code |= CLIP_OUTCODE_BOTTOM;
    else if (p.y > cr.x + cr.w - 1) code |= CLIP_OUTCODE_TOP;
    return code;
}

bool clip_line(vec2f* p1, vec2f* p2, recti_t cr) {
    u32 code1 = compute_outcode(*p1, cr);
    u32 code2 = compute_outcode(*p2, cr);

    f32 xmin = (f32)recti_xmin(&cr);
    f32 xmax = (f32)recti_xmax(&cr);
    f32 ymin = (f32)recti_ymin(&cr);
    f32 ymax = (f32)recti_ymax(&cr);

    while (true) {
        if (!(code1 | code2)) {
            return true;
        } else if ((code1 & code2)) {
            return false;
        } else {
            vec2f d = vec2f_sub(*p2, *p1);
            u32* code;
            vec2f* p; 
            if (code1 > code2) {
                code = &code1;
                p = p1;
            } else {
                code = &code2;
                p = p2;
            }           

            const f32 x1 = p1->x;
            const f32 y1 = p1->y;
            const f32 x2 = p2->x;
            const f32 y2 = p2->y;

            if (*code & CLIP_OUTCODE_TOP) {
                p->y = ymax;
                p->x = lerp_f32(x1, x2, (ymax - y1) / d.y);
            } else if (*code & CLIP_OUTCODE_BOTTOM) {
                p->y = ymin;
                p->x = lerp_f32(x1, x2, (ymin - y1) / d.y);
            } else if (*code & CLIP_OUTCODE_RIGHT) {
                p->x = xmax;
                p->y = lerp_f32(y1, y2, (xmax - x1) / d.x);
            } else if (*code & CLIP_OUTCODE_LEFT) {
                p->x = xmin;
                p->y = lerp_f32(y1, y2, (xmin - x1) / d.x);
            }
            *code = compute_outcode(*p, cr);
        }
    }
    return false;
}

static void gfxa_draw_line_u8_impl2(frame_buffer_t* fb, vec2f p1, vec2f p2, u8 color) {
    vec2f diff = vec2f_sub(p2, p1);

    if (vec2f_is_zero(diff)) {
        gfxa_draw_pixel_u8(fb, vec2f_round(p1), color);
        return;
    }

    f32 dist = vec2f_max_element(vec2f_abs(diff));
    s32 count = (s32)dist + 1;
    vec2f inc = vec2f_smul(diff, 1/dist);
    vec2f p = p1;
    
    while(count--) {
        vec2i p_i = vec2f_round(p);
        gfxa_draw_pixel_u8(fb, p_i, color);
        p = vec2f_add(p, inc);
    }
}

static void gfxa_draw_line_u8_impl3(frame_buffer_t* fb, vec2f p1, vec2f p2, u8 color) {
    vec2f diff = vec2f_sub(p2, p1);

    if (vec2f_is_zero(diff)) {
        gfxa_draw_pixel_u8(fb, vec2f_round(p1), color);
        return;
    }

    f32 dist = vec2f_max_element(vec2f_abs(diff));
    s32 count = (s32)dist + 1;
    s32 inc_x = (s32)(diff.x / dist * 256.0f);    
    s32 inc_y = (s32)(diff.y / dist * 256.0f);    
    s32 x = (s32)(p1.x * 256.0f);
    s32 y = (s32)(p1.y * 256.0f);

    while(count--) {
        gfxa_draw_pixel_u8(fb, (vec2i){(x + 128) / 256, (y + 128) / 256}, color);
        x += inc_x;
        y += inc_y;
    }
}

static void gfxa_draw_line_u8_impl(frame_buffer_t* fb, vec2f p1, vec2f p2, u8 color) {
    f32 dx = (p2.x - p1.x);
    f32 dy = (p2.y - p1.y);

    if (dx == 0.0f && dy == 0.0f) {
        s32 x = (s32)(p1.x + 0.5f);
        s32 y = (s32)(p1.y + 0.5f);
        fb->data[y * fb->width + x] = color;
    } else if (fabs(dx) > fabs(dy)) {
        f32 x1, x2, y1;
        if (p1.x < p2.x) {
            x1 = p1.x;
            x2 = p2.x;
            y1 = p1.y;
        } else {
            x1 = p2.x;
            x2 = p1.x;
            y1 = p2.y;
            dx *= -1.0f;
            dy *= -1.0f;
        }        

        f32 dydx = dy / dx;
        f32 x = x1;
        f32 y = y1;
        while (x <= x2) {
            s32 xi = (s32)(x + 0.5f);
            s32 yi = (s32)(y + 0.5f);
            fb->data[yi * fb->width + xi] = color;
            x += 1.0f;
            y += dydx;
        }
    } else {
        f32 x1, y1, y2;
        if (p1.y < p2.y) {
            x1 = p1.x;
            y1 = p1.y;
            y2 = p2.y;
        } else {
            x1 = p2.x;
            y1 = p2.y;
            y2 = p1.y;
            dx *= -1.0f;
            dy *= -1.0f;
        }        

        f32 dxdy = dx / dy;
        f32 x = x1;
        f32 y = y1;
        while (y <= y2) {
            s32 xi = (s32)(x + 0.5f);
            s32 yi = (s32)(y + 0.5f);
            fb->data[yi * fb->width + xi] = color;
            x += dxdy;
            y += 1.0f;
        }
    }

}


void gfxa_draw_line_u8(frame_buffer_t* fb, vec2i p1, vec2i p2, u8 color) {
    vec2f p1f = {(f32)p1.x, (f32)p1.y};
    vec2f p2f = {(f32)p2.x, (f32)p2.y};
    recti_t cr = {0, 0, fb->width, fb->height};
    bool visible = clip_line(&p1f, &p2f, cr);
    if (visible) {
        gfxa_draw_line_u8_impl3(fb, p1f, p2f, color);
    }
}   

void gfxa_draw_lines_u8(frame_buffer_t* fb, vec2i* points, u32 count, u8 color, bool close_shape) {
    vec2i first_point = *points++;
    vec2i current_point = first_point;
    vec2i next_point = first_point; 
    while (--count) {
        current_point = next_point;
        next_point = *points;
        gfxa_draw_line_u8(fb, current_point, next_point, color);
        points++;
    }

    if (close_shape) {
        gfxa_draw_line_u8(fb, next_point, first_point, color);
    }
}

void gfxa_fill_rect_u8(frame_buffer_t* fb, recti_t* r, u8 color) {
    s32 x1 = max_s32(recti_xmin(r), 0);
    s32 x2 = min_s32(recti_xmax(r), (s32)fb->width - 1);
    s32 y1 = max_s32(recti_ymin(r), 0);
    s32 y2 = min_s32(recti_ymax(r), (s32)fb->height - 1);
    s32 height = y2 - y1 + 1;
    s32 width = x2 - x1 + 1;    
    u32 color_4 = dup_u8_u32(color);
    u64 color_8 = dup_u8_u64(color);
    u8* row_ptr = fb->data + y1 * fb->width + x1;
    
    for (s32 i = 0; i < height; ++i) {
        u8* dst = row_ptr;
        s32 j = width;
        while(j > 0 && (u64)dst % 8) {
            *dst++ = color;
            j--;
        }
        while (j / 8 > 0) {
            *(u64*)dst = color_8;
            dst += 8;
            j -= 8;
        }
        while (j / 4 > 0) {
            *(u32*)dst = color_4;
            dst += 4;
            j -= 4;
        }        
        while (j > 0) {
            *dst++ = color;
            j--;
        }
        row_ptr += fb->width;
    }
}

void gfxa_fill_horizontal_span_u8(frame_buffer_t* fb, s32 y, s32 x1, s32 x2, u8 color) {
    assert(y >= 0 && y < (s32)fb->height);
    assert(x1 >= 0 && x1 < (s32)fb->width);
    assert(x2 >= 0 && x2 < (s32)fb->width);
    assert(x1 <= x2);
    u32 count = x2 - x1 + 1;
    u8* dst = frame_buffer_pixel_address_u8(fb, x1, y);
    memset(dst, color, count);
}

void gfxa_fill_horizontal_span_wrapping_u8(frame_buffer_t* fb, s32 y, s32 x1, s32 x2, u8 color) {
    if (x2 < x1) swp_s32(&x1, &x2);
    x1 = x1 % fb->width;
    x2 = x2 % fb->width;
    y = y % fb->height;    

    if (x1 < x2) {
        u32 count = x2 - x1 + 1;
        u8* dst = fb->data + y * fb->width + x1;
        memset(dst, color, count);
    } else if (x1 > x2) {
        u32 count1 = fb->width - x1 + 1;
        u32 count2 = x2 + 1;
        u8* row_ptr = fb->data + y * fb->width;
        memset(row_ptr + x1, color, count1);
        memset(row_ptr, color, count2);
    }
}

void gfxa_fill_vertical_span_u8(frame_buffer_t* fb, s32 y1, s32 y2, s32 x, u8 color) {
    assert(x >= 0 && x < (s32)fb->width);
    assert(y1 >= 0 && y1 < (s32)fb->height);
    assert(y2 >= 0 && y2 < (s32)fb->height);
    s32 height = y2 - y1 + 1;
    u8* dst = frame_buffer_pixel_address_u8(fb, x, y1);
    for (int i = 0; i < height; i++) {
        *dst = color;
        dst += fb->width;
    }
}

void gfxa_draw_rect_u8(frame_buffer_t* fb, recti_t* r, u8 color) {
    s32 xmin = recti_xmin(r);
    s32 xmax = recti_xmax(r);
    s32 ymin = recti_ymin(r);
    s32 ymax = recti_ymax(r);    

    recti_t cr = frame_buffer_get_clipping_rect(fb);

    if (xmin >= cr.x + cr.w || xmax < cr.x || ymin >= cr.y + cr.h || ymax < cr.y) {
        return;
    }
    
    s32 x1 = max_s32(xmin, cr.x);
    s32 x2 = min_s32(xmax, cr.x + cr.w - 1);
    s32 y1 = max_s32(ymin, cr.y);
    s32 y2 = min_s32(ymax, cr.y + cr.h - 1);

    if (ymin >= 0) gfxa_fill_horizontal_span_u8(fb, ymin, x1, x2, color);
    if (ymax < (s32)fb->height) gfxa_fill_horizontal_span_u8(fb, ymax, x1, x2, color);
    if (xmin >= 0) gfxa_fill_vertical_span_u8(fb, y1, y2, xmin, color);
    if (xmax < (s32)fb->width) gfxa_fill_vertical_span_u8(fb, y1, y2, xmax, color);
}

void gfxa_scan_convert_triangle(frame_buffer_t* fb, vec2i p1, vec2i p2, vec2i p3) {
    // sort the points
    if (p2.y < p1.y) vec2i_swp(&p1, &p2);
    if (p3.y < p1.y) vec2i_swp(&p1, &p3);
    if (p3.y < p2.y) vec2i_swp(&p2, &p3);

    s32 xmin = min_s32(p1.x, min_s32(p2.x, p3.x));
    s32 xmax = max_s32(p1.x, max_s32(p2.x, p3.x));
    s32 ymin = p1.y;
    s32 ymax = p3.y;

    frame_buffer_clear_span_buffer(fb);

    if (xmax < 0 || xmin >= (s32)fb->width || ymax < 0 || ymin >= (s32)fb->height) return;

    if (xmin == xmax) {
        for (s32 y = ymin; y <= ymax; ++y) {
            frame_buffer_push_span(fb, y, xmin, xmax);
        }
        return;
    }
    
    if (ymin == ymax) {
        frame_buffer_push_span(fb, ymin, xmin, xmax);
        return;
    }
    
    s32 x1 = p1.x * 256;
    s32 x2 = x1;
    s32 dx1dy = 0;
    s32 dx2dy = 0;
    s32 y_end = 0;
    if (p1.y == p2.y) {
        x2 = p2.x * 256;
        y_end = p3.y;
        dx1dy = (p3.x - p1.x) * 256 / (p3.y - p1.y);
        dx2dy = (p3.x - p2.x) * 256 / (p3.y - p2.y);
    } else {
        y_end = p2.y;
        dx1dy = (p2.x - p1.x) * 256 / (p2.y - p1.y);
        dx2dy = (p3.x - p1.x) * 256 / (p3.y - p1.y);
    }
    
    s32 y = ymin;
    while (y <= ymax) {
        frame_buffer_push_span(fb, y, (x1 + 128) / 256, (x2 + 128) / 256);
        if (y == y_end && y != ymax) {
            dx1dy = 256 * (p3.x - p2.x) / (p3.y - p2.y);
        }
        x1 += dx1dy;
        x2 += dx2dy;
        y++;
    }
} 

void gfxa_fill_triangle_u8(frame_buffer_t* fb, vec2i p1, vec2i p2, vec2i p3, u8 color) {
    gfxa_scan_convert_triangle(fb, p1, p2, p3);
    if (fb->use_clipping) {
        gfxa_fill_spans_clipped_u8(fb, color);
    } else {
        gfxa_fill_spans_wrapped_u8(fb, color);
    }
}

void gfxa_fill_spans_clipped_u8(frame_buffer_t* fb, u8 color) {
    grv_span_buffer_t* spans = &fb->span_buffer;
    if (spans->count == 0) return;

    s32 y1 = spans->y_start;
    s32 y2 = y1 + spans->count - 1;

    recti_t clip_rect = frame_buffer_get_clipping_rect(fb);
    s32 xmin = recti_xmin(&clip_rect);
    s32 xmax = recti_xmax(&clip_rect);
    s32 ymin = recti_ymin(&clip_rect);
    s32 ymax = recti_ymax(&clip_rect);

    if (y2 < ymin || y1 > ymax) return;

    y1 = max_s32(y1, ymin);
    y2 = min_s32(y2, ymax);    

    s32 idx = y1 - spans->y_start;
    s32 count_y = y2 - y1 + 1;
    grv_span_t* span = spans->spans + idx;
    u8* row_ptr = frame_buffer_pixel_address_u8(fb, 0, y1);

    while(count_y--) {
        s32 x1 = max_s32(span->x1, xmin);
        s32 x2 = min_s32(span->x2, xmax);
        s32 count_x = x2 - x1 + 1;
        memset(row_ptr + x1, color, count_x);
        row_ptr += fb->width;
        span++;
    }    
}

void gfxa_fill_spans_wrapped_u8(frame_buffer_t* fb, u8 color) {
    (void) fb;
    (void) color;
}