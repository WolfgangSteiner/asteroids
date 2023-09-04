#ifndef GRV_MATH_H
#define GRV_MATH_H

#include "grv/grv_base.h"

#define PI_F32  3.141592653589793f
#define TWO_PI_F32 (2.0f * PI_F32)

typedef struct {
    f32 sin;
    f32 cos;
} sin_cos_t;


f32 unit_sin_f32(f32 arg);
f32 unit_cos_f32(f32 arg);

f32 unit_sin_u16(u16 arg);
f32 unit_cos_u16(u16 arg);

f32 unit_sin_lut_u16(u16 arg);
f32 unit_cos_lut_u16(u16 arg);

sin_cos_t unit_sin_cos_u16(u16 arg);
sin_cos_t unit_sin_cos_lut_u16(u16 arg);


f32 sqr_f32(f32 a);

s32 round_f32(f32 a);
f32 lerp_f32(f32 a, f32 b, f32 t);

s32 min_s32(s32 a, s32 b);
s32 max_s32(s32 a, s32 b);

u32 min_u32(u32 a, u32 b);
u32 max_u32(u32 a, u32 b);

f32 min_f32(f32 a, f32 b);
f32 max_f32(f32 a, f32 b);

u32 dup_u8_u32(u8 a);
u64 dup_u8_u64(u8 a);

f32 abs_f32(f32 a);

void swp_s32(s32* a, s32* b);

#endif