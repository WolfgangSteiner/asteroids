#include "grv_profile.h"
#include "grv/grv_base.h"
#include "grv/math.h"


int main(void) {
    u32 iter = 1 << 26;
    GRV_PROFILE(unit_sin_cos_lut_u16, unit_sin_cos_lut_u16(ITER % 4096), iter);
    GRV_PROFILE(unit_sin_cos_u16, unit_sin_cos_u16(ITER % 4096), iter);
}