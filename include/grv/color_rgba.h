#ifndef COLOR_RGBA_H
#define COLOR_RGBA_H

#include "grv/grv_base.h"

typedef union {
    struct {
        u8 r, g, b, a;
    };
    u32 rgba;
} color_rgba_t;


#endif