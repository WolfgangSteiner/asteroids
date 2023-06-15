#include "vec2i.h"

void vec2i_swp(vec2i* a, vec2i* b) { vec2i swp = *a; *a = *b; *b = swp; }