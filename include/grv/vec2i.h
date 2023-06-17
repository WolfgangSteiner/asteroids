#ifndef VEC2I_H
#define VEC2I_H

#include "grv/base_defines.h"

typedef struct vec2i_s {
    s32 x, y;
} vec2i;

void vec2i_swp(vec2i* a, vec2i* b);

#endif