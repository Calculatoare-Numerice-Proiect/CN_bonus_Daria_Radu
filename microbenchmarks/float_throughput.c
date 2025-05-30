#include <stdio.h>
#include <stdint.h>
#include <time.h>

#define ITER (1000000000)

int main() {
    double x = 1.0, y = 2.0, z = 0.0;
    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);
    for (uint64_t i = 0; i < ITER; i++) {
        z += x * y;   // multiply-add floating
        x /= y;       // divide
    }
    clock_gettime(CLOCK_MONOTONIC, &t1);
    double elapsed = (t1.tv_sec - t0.tv_sec)
                     + (t1.tv_nsec - t0.tv_nsec) * 1e-9;
    printf("ops=%llu time=%.6f\n", (unsigned long long)ITER * 2, elapsed);
    return 0;
}