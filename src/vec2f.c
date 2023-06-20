#include "grv/vec2f.h"
#include "grv/vec2i.h"
#include "grv/math.h"

bool vec2f_is_zero(vec2f a) { return a.x == 0.0f && a.y == 0.0f; }
vec2f vec2f_abs(vec2f a) { return (vec2f){abs_f32(a.x), abs_f32(a.y)}; }
f32 vec2f_max_element(vec2f a) { return max_f32(a.x, a.y); }
f32 vec2f_min_element(vec2f a) { return min_f32(a.x, a.y); }
vec2f vec2f_add(vec2f a, vec2f b) { return (vec2f){a.x + b.x, a.y + b.y}; }
vec2f vec2f_sub(vec2f a, vec2f b) { return (vec2f){a.x - b.x, a.y - b.y}; }
vec2f vec2f_smul(vec2f a, f32 b) { return (vec2f){a.x * b, a.y * b}; }
vec2i vec2f_round(vec2f a) { return (vec2i){round_f32(a.x), round_f32(a.y)}; }

vec2f vec2f_rot(vec2f a, f32 rot) {
    f32 sinv = unit_sin_f32(rot);
    f32 cosv = unit_cos_f32(rot);
    return (vec2f){ cosv * a.x - sinv * a.y, sinv * a.x + cosv * a.y };
}

vec2f vec2f_lerp(vec2f a, vec2f b, f32 t) {
    return vec2f_add(a, vec2f_smul(vec2f_sub(b, a), t));
    //return (vec2f){a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t};
}
