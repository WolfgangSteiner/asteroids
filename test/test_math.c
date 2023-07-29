#include "grv_test.h"
#include "grv/math.h"

GRV_TEST_BEGIN(grv_math_sin_cos_lut)
    for (int i = 0; i < 2048; i+=16) {
        f32 rad = (f32)i / 4096.0f * TWO_PI_F32;
        f32 sinv = sinf(rad);
        f32 cosv = cosf(rad);
        sin_cos_t sc = unit_sin_cos_lut_u16(i);
        //printf("rad: %f pi\n", rad / PI_F32);
        GRV_TEST_ASSERT_CLOSE(sc.sin, sinv, 0.01f);
        GRV_TEST_ASSERT_CLOSE(sc.cos, cosv, 0.01f);
    }
GRV_TEST_END()

int main(void) {
    GRV_TEST_INIT(grv_math);
    GRV_TEST_RUN(grv_math_sin_cos_lut);
    GRV_TEST_FINISH();
}