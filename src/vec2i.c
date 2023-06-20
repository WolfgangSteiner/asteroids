#include "grv/vec2i.h"

vec2i vec2i_add(vec2i a, vec2i b) { return (vec2i){ a.x + b.x , a.y + b.y}; }
vec2i vec2i_sub(vec2i a, vec2i b) { return (vec2i){ a.x - b.x , a.y - b.y}; }
void vec2i_swp(vec2i* a, vec2i* b) { vec2i swp = *a; *a = *b; *b = swp; }