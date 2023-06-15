#include "gfxa.h"
#include "recti.h"
#include "vec2f.h"
#include "grv_math.h"
#include <math.h>
#include <assert.h>

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
    vec2f diff = vec2f_abs(vec2f_sub(p2, p1));

    if (vec2f_is_zero(diff)) {
        gfxa_draw_pixel_u8(fb, vec2f_round(p1), color);
        return;
    }

    f32 dist = vec2f_max_element(diff);
    s32 N = (s32)dist;

    for (int i = 0; i <= N; ++i) {
        vec2i p = vec2f_round(vec2f_lerp(p1, p2, (f32)i / dist));
        gfxa_draw_pixel_u8(fb, p, color);
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
        f32 x1, x2, y1, y2;
        if (p1.x < p2.x) {
            x1 = p1.x;
            x2 = p2.x;
            y1 = p1.y;
            y2 = p2.y;
        } else {
            x1 = p2.x;
            x2 = p1.x;
            y1 = p2.y;
            y2 = p1.y;
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
        f32 x1, x2, y1, y2;
        if (p1.y < p2.y) {
            x1 = p1.x;
            x2 = p2.x;
            y1 = p1.y;
            y2 = p2.y;
        } else {
            x1 = p2.x;
            x2 = p1.x;
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
        gfxa_draw_line_u8_impl2(fb, p1f, p2f, color);
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

void gfxa_fill_horizontal_span_clipped_u8(frame_buffer_t* fb, s32 y, s32 x1, s32 x2, u8 color) {
    x1 = max_s32(x1, 0);
    x2 = min_s32(x2, fb->width - 1);
    s32 width = x2 - x1 + 1;
    u8* dst = fb->data + y * fb->width + x1;
    u64 color_8 = dup_u8_u64(color);

    while (width && (u64)dst % 8) {
        *dst++ = color;
        width--;
    }    
    while (width / 8) {
        *(u64*)dst = color_8;
        dst += 8;
        width -= 8;     
    }
    while (width) {
        *dst++ = color;
        width--;
    }
}

void gfxa_fill_vertical_span_clipped_u8(frame_buffer_t* fb, s32 y1, s32 y2, s32 x, u8 color) {
    y1 = max_s32(0, y1);
    y2 = min_s32(y2, fb->height - 1);
    s32 height = y2 - y1 + 1;
    u8* dst = fb->data + y1 * fb->width + x;
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

    if (xmin >= (s32)fb->width || xmax < 0 || ymin >= (s32)fb->height || ymax < 0) {
        return;
    }
    
    if (ymin >= 0) gfxa_fill_horizontal_span_clipped_u8(fb, ymin, xmin, xmax, color);
    if (ymax < (s32)fb->height) gfxa_fill_horizontal_span_clipped_u8(fb, ymax, xmin, xmax, color);
    if (xmin >= 0) gfxa_fill_vertical_span_clipped_u8(fb, ymin, ymax, xmin, color);
    if (xmax < (s32)fb->width) gfxa_fill_vertical_span_clipped_u8(fb, ymin, ymax, xmax, color);
}

void gfxa_fill_triangle_u8(frame_buffer_t* fb, vec2i p1, vec2i p2, vec2i p3, u8 color) {
    s32 xmin = min_s32(p1.x, min_s32(p2.x, p3.x));
    s32 xmax = max_s32(p1.x, max_s32(p2.x, p3.x));
    s32 ymin = min_s32(p1.y, min_s32(p2.y, p3.y));
    s32 ymax = max_s32(p1.y, max_s32(p2.y, p3.y));
    if (xmax < 0 || xmin >= (s32)fb->width || ymax < 0 || ymin >= (s32)fb->height) return;

    // sort the points
    if (p2.y < p1.y) vec2i_swp(&p1, &p2);
    if (p3.y < p1.y) vec2i_swp(&p1, &p3);
    if (p3.y < p2.y) vec2i_swp(&p2, &p3);
    
    if (p1.y == p2.y) {
        if (p2.x < p1.x) vec2i_swp(&p1, &p2);  
    } else if (p3.x < p2.x) vec2i_swp(&p2, &p3); 

    assert(p1.y <= p2.y);
    assert(p1.y <= p3.y);
    assert((p1.y == p2.y) || (p2.x <= p3.x));

    f32 x1 = (f32)p1.x;
    f32 x2 = (f32)p1.x;
    f32 dx1dy = 0.0f;
    f32 dx2dy = 0.0f;
    if (p1.y == p2.y) {
        x2 = (f32)p2.x;
        dx1dy = (f32)(p3.x - p1.x) / (f32)(p3.y - p1.y);
        dx2dy = (f32)(p3.x - p2.x) / (f32)(p3.y - p2.y);
    } else {
        dx1dy = (f32)(p2.x - p1.x) / (f32)(p2.y - p1.y);
        dx2dy = (f32)(p3.x - p1.x) / (f32)(p3.y - p1.y);
    }

    for (s32 y = ymin; y <= ymax; ++y) {
        gfxa_fill_horizontal_span_clipped_u8(fb, y, round_f32(x1), round_f32(x2), color);
        if (p2.y != p3.y && p1.y != p2.y && y == p2.y) { dx1dy = (f32)(p3.x - p2.x) / (f32)(p3.y - p2.y); }
        else if (p2.y != p3.y && y == p3.y) { dx2dy = (f32)(p2.x - p3.x) / (f32)(p2.y - p3.y); }
        x1 += dx1dy;
        x2 += dx2dy;
    }
} 