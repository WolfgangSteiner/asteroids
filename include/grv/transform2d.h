#ifndef TRANSFORM2D_H
#define TRANSFORM2D_H

#include "vec2i.h"

typedef struct {
    f32 rotation;
    f32 scale;
    vec2i translation;
} transform2d_t;


vec2i transform2d_translate(vec2i v, vec2i t);
vec2i transform2d_rotate(vec2i v, f32 rotation);
vec2i transform2d_scale(vec2i v, f32 scale);
vec2i transform2d(transform2d_t* t, vec2i v);
void transform2d_apply(transform2d_t* t, vec2i* dst, vec2i* src);
void transform2d_apply_array(transform2d_t* t, vec2i* points, size_t count);
vec2i transform2d(transform2d_t* t, vec2i v);

#endif