#include <stdio.h>
#include <stdint.h>
#include <time.h>

#define ITER (1000000000)

int main() {
    uint64_t a = 1, b = 2, c = 0;
    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);
    for (uint64_t i = 0; i < ITER; i++) {
        c += a * b;   // multiply-add pattern
        a ^= b;       // integer operation
    }
    clock_gettime(CLOCK_MONOTONIC, &t1);
    double elapsed = (t1.tv_sec - t0.tv_sec)
                     + (t1.tv_nsec - t0.tv_nsec) * 1e-9;
    // printf("ops=%llu time=%.6f\n", (unsigned long long)ITER * 2, elapsed);
    printf("ops,time_s\n");
    printf("%llu,%.6f\n", (unsigned long long)ITER * 2, elapsed);
    return 0;
}