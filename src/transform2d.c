#include "grv/transform2d.h"
#include "grv/math.h"
#include "grv/vec2f.h"

vec2i transform2d_translate(vec2i v, vec2i t) {
   return (vec2i){v.x + t.x, v.y + t.y}; 
}


vec2i transform2d(transform2d_t* transform, vec2i v) {
    const f32 r = transform->rotation;
    const f32 s = transform->scale;
    const vec2i t = transform->translation;

    f32 cosv = unit_cos_f32(r) * s;
    f32 sinv = unit_sin_f32(r) * s;

    vec2f res;
    res.x = cosv * v.x - sinv * v.y + t.x;
    res.y = sinv * v.x + cosv * v.y + t.y;

    return vec2f_round(res);
}

vec2i transform2d_rotate(vec2i v, f32 rotation) {
    const f32 cosv = unit_cos_f32(rotation);
    const f32 sinv = unit_sin_f32(rotation);

    vec2f res;
    res.x = cosv * v.x - sinv * v.y;
    res.y = sinv * v.x + cosv * v.y;    

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

    f32 cosv = unit_cos_f32(r) * s;
    f32 sinv = unit_sin_f32(r) * s;

    while (count--) {
        vec2i v = *points;
        vec2f res;
        res.x = cosv * v.x - sinv * v.y + t.x;
        res.y = sinv * v.x + cosv * v.y + t.y;
        *points++ = vec2f_round(res);
    }
}