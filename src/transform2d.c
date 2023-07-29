#include "grv/transform2d.h"
#include "grv/math.h"
#include "grv/vec2f.h"

vec2i transform2d_translate(vec2i v, vec2i t) {
   return (vec2i){v.x + t.x, v.y + t.y}; 
}


vec2i transform2d(transform2d_t* transform, vec2i v) {
    const u16 r = transform->rotation;
    const f32 s = transform->scale;
    const vec2i t = transform->translation;

    sin_cos_t sc = unit_sin_cos_lut_u16(r);
    const f32 cosv = sc.cos * s;
    const f32 sinv = sc.sin * s;
    vec2f res;
    res.x = cosv * v.x - sinv * v.y + t.x;
    res.y = sinv * v.x + cosv * v.y + t.y;

    return vec2f_round(res);
}

vec2i transform2d_rotate(vec2i v, u16 rotation) {
    sin_cos_t sc = unit_sin_cos_lut_u16(rotation);

    vec2f res;
    res.x = sc.cos * v.x - sc.sin * v.y;
    res.y = sc.sin * v.x + sc.cos * v.y;    

    return vec2f_round(res);
}

vec2i transform2d_scale(vec2i v, f32 scale) {
    return (vec2i){(s32)(v.x * scale), (s32)(v.y * scale)};
}

void transform2d_apply(transform2d_t* t, vec2i* dst, vec2i* src) {
    *dst = transform2d(t, *src);
}

void transform2d_apply_array(transform2d_t* transform, vec2i* points, size_t count) {
    const f32 r = transform->rotation;
    const f32 s = transform->scale;
    const vec2i t = transform->translation;

    sin_cos_t sc = unit_sin_cos_lut_u16(r);
    const f32 cosv = sc.cos * s;
    const f32 sinv = sc.sin * s;

    while (count--) {
        vec2i v = *points;
        vec2f res;
        res.x = cosv * v.x - sinv * v.y + t.x;
        res.y = sinv * v.x + cosv * v.y + t.y;
        *points++ = vec2f_round(res);
    }
}