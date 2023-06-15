#ifndef RECTI_H
#define RECTI_H

#include "base_defines.h"

typedef struct {
   s32 x, y, w, h;
} recti_t;

s32 recti_xmin(recti_t* a);
s32 recti_xmax(recti_t* a);
s32 recti_ymin(recti_t* a);
s32 recti_ymax(recti_t* a);

#endif
