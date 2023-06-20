#ifndef VEC2F_H
#define VEC2F_H

#include "grv/common.h"

struct vec2i_s;

typedef struct {
    f32 x, y;
} vec2f;

bool vec2f_is_zero(vec2f a);
vec2f vec2f_abs(vec2f a);
f32 vec2f_max_element(vec2f a);
f32 vec2f_min_element(vec2f a);

vec2f vec2f_add(vec2f a, vec2f b);
vec2f vec2f_sub(vec2f a, vec2f b);
vec2f vec2f_smul(vec2f a, f32 b);

// rotate a vector, rot is fraction of unit circle
vec2f vec2f_rot(vec2f a, f32 rot);

// linear interpolation
vec2f vec2f_lerp(vec2f a, vec2f b, f32 t);

// rounding operation
struct vec2i_s vec2f_round(vec2f a);


#endif