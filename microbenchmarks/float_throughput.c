#include <stdio.h>
#include <stdint.h>
#include <time.h>

#define SIZES_COUNT 4
static const uint64_t ITERS[SIZES_COUNT] = {
    100000000ULL,
    500000000ULL,
    1000000000ULL,
    2000000000ULL
};

int main() {
    printf("ops,time_s\n");
    for (int i = 0; i < SIZES_COUNT; i++) {
        uint64_t ITER = ITERS[i];
        volatile double x = 1.0, y = 2.0, z = 0.0;

        struct timespec t0, t1;
        clock_gettime(CLOCK_MONOTONIC, &t0);
        for (uint64_t j = 0; j < ITER; j++) {
            z += x * y;
            x /= y;
        }
        clock_gettime(CLOCK_MONOTONIC, &t1);

        double elapsed = (t1.tv_sec - t0.tv_sec)
                       + (t1.tv_nsec - t0.tv_nsec) * 1e-9;
        printf("%llu,%.6f\n", (unsigned long long)(ITER * 2), elapsed);
    }
    return 0;
}
