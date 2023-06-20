#include <math.h>
#include "grv/math.h"

f32 unit_sin_f32(f32 arg) {
    return sinf(arg * TWO_PI_F32);
}

f32 unit_cos_f32(f32 arg) {
    return cosf(arg * TWO_PI_F32);
}

f32 sqr_f32(f32 a) { return a * a; }

s32 round_f32(f32 a) { return (s32)roundf(a); };
f32 lerp_f32(f32 a, f32 b, f32 t) { return a + t * (b - a); }

s32 min_s32(s32 a, s32 b) { return a < b ? a : b; }
s32 max_s32(s32 a, s32 b) { return a > b ? a : b; }

u32 min_u32(u32 a, u32 b) { return a < b ? a : b; }
u32 max_u32(u32 a, u32 b) { return a > b ? a : b; }

f32 min_f32(f32 a, f32 b) { return a < b ? a : b; }
f32 max_f32(f32 a, f32 b) { return a > b ? a : b; }

u32 dup_u8_u32(u8 a) {
    u32 res = (u32)a;
    res |= res << 8;
    res |= res << 16;
    return res;
};

u64 dup_u8_u64(u8 a) {
    u64 res = (u64)a;
    res |= res << 8;
    res |= res << 16;
    res |= res << 32;
    return res;
};

f32 abs_f32(f32 a) { return a < 0.0f ? -a : a; }

void swp_s32(s32* a, s32* b) { s32 c = *a; *a = *b; *b = c; };