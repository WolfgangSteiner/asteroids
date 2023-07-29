#include <math.h>
#include "grv/math.h"
#include "sin_cos_lut.c"
#include <assert.h>

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

f32 unit_sin_u16(u16 arg) {
    f32 rad = (f32)arg / 65536.0f * TWO_PI_F32;
    return sinf(rad);
}

f32 unit_cos_u16(u16 arg) {
    f32 rad = (f32)arg / 65536.0f * TWO_PI_F32;
    return cosf(rad);
}

#define SIN_COS_STEPS 4096
#define LUT_SIZE (SIN_COS_STEPS / 8)

f32 unit_sin_lut_u16(u16 arg) {
    sin_cos_t sc = unit_sin_cos_lut_u16(arg);
    return sc.sin;
}

f32 unit_cos_lut_u16(u16 arg) {
    sin_cos_t sc = unit_sin_cos_lut_u16(arg);
    return sc.cos;
}

sin_cos_t unit_sin_cos_u16(u16 arg) {
    return (sin_cos_t){unit_sin_u16(arg), unit_cos_u16(arg)};
}

sin_cos_t unit_sin_cos_lut_u16(u16 arg) {
    arg = arg & (SIN_COS_STEPS - 1);
    u32 sin_idx = 0;
    u32 cos_idx = 1;
    f32 sin_sign = 1.0f;
    f32 cos_sign = 1.0f;

    // shift down to lower half
    if (arg >= SIN_COS_STEPS / 2) {
        arg -= SIN_COS_STEPS / 2;
        sin_sign *= -1.0f;
        cos_sign *= -1.0f;
    }

    // shift to first quadrant
    if (arg >= SIN_COS_STEPS / 4) {
        arg -= SIN_COS_STEPS / 4;
        cos_sign *= -1.0f;
        sin_idx = 1;
        cos_idx = 0;
    }

    // reflect over pi/4
    if (arg > SIN_COS_STEPS / 8) {
        arg = SIN_COS_STEPS / 4 - arg;
        sin_idx = (sin_idx + 1) % 2;
        cos_idx = (cos_idx + 1) % 2;
    }
    assert(arg <= LUT_SIZE);
    float* sin_cos_val = unit_sin_cos_lut_4096[arg];
    return (sin_cos_t){sin_sign * sin_cos_val[sin_idx], cos_sign * sin_cos_val[cos_idx]};
}