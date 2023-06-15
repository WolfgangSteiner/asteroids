#include "grv/transform2d.h"
#include "grv/math.h"

vec2i transform2d_translate(vec2i v, vec2i t) {
   return (vec2i){v.x + t.x, v.y + t.y}; 
}


vec2i transform2d(transform2d_t* t, vec2i v) {
    const f32 rotation = t->rotation;
    vec2i res = v;

    if ((f32)((s32)rotation) != rotation) {
        res = transform2d_rotate(res, rotation);
    }

    res = transform2d_scale(res, t->scale);
    res = transform2d_translate(res, t->translation);

    return res;
}

vec2i transform2d_rotate(vec2i v, f32 rotation) {
    const f32 cosv = unit_cos_f32(rotation);
    const f32 sinv = unit_sin_f32(rotation);

    f32 x = cosv * v.x - sinv * v.y;
    f32 y = sinv * v.x + cosv * v.y;    

    return (vec2i) { (s32)(x + 0.5f), (s32)(y + 0.5f) };
}

vec2i transform2d_scale(vec2i v, f32 scale) {
    return (vec2i){(s32)(v.x * scale), (s32)(v.y * scale)};
}

void transform2d_apply(transform2d_t* t, vec2i* dst, vec2i* src) {
    *dst = transform2d(t, *src);
}

void transform2d_apply_array(transform2d_t* t, vec2i* points, size_t count) {
    for (size_t i = 0; i < count; ++i) {
        points[i] = transform2d(t, points[i]);   
    }
}