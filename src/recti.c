#include "grv/recti.h"


s32 recti_xmin(recti_t* a) { return a->x; }
s32 recti_xmax(recti_t* a) { return a->x + a->w - 1; }
s32 recti_ymin(recti_t* a) { return a->y; }
s32 recti_ymax(recti_t* a) { return a->y + a->h - 1; }