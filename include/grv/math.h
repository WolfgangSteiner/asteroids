#ifndef MGE_MATH_H
#define MGE_MATH_H

#include "grv/base_defines.h"

#define PI_F32  3.141592653589793f
#define TWO_PI_F32 (2.0f * PI_F32)

f32 unit_sin_f32(f32 arg);
f32 unit_cos_f32(f32 arg);

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

#endif