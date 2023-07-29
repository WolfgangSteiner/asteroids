#include <math.h>
#include <grv/math.h>
#include <stdio.h>

void create_lut(int num_steps) {
    printf("f32 unit_sin_cos_lut_%d[%d][2] = {\n", num_steps, num_steps/8 + 1);
    int i = 0;
    while (i <= num_steps / 8) {
        f32 rad = (f32)i / (f32)num_steps * TWO_PI_F32;
        f32 sinv = sinf(rad);
        f32 cosv = cosf(rad);
        printf("{%ff, %ff}", sinv, cosv);
        if (i < num_steps / 8) printf(", ");
        if (i % 4 == 3) printf("\n");
        i++;
    }
    printf("};\n");
}

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;
    create_lut(4096);
}

