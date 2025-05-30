#include <stdio.h>
#include <stdint.h>
#include <time.h>

#define SIZES_COUNT 4
// define four iteration counts (you can tune these)
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
        volatile uint64_t a = 1, b = 2, c = 0;  // volatile to prevent optimize-away

        struct timespec t0, t1;
        clock_gettime(CLOCK_MONOTONIC, &t0);
        for (uint64_t j = 0; j < ITER; j++) {
            c += a * b;
            a ^= b;
        }
        clock_gettime(CLOCK_MONOTONIC, &t1);

        // double elapsed = (t1.tv_sec - t0.tv_sec)
        //                + (t1.tv_nsec - t0.tv_nsec) * 1e-9;
        // // two ops per iteration: multiply and xor
        // printf("%llu,%.6f\n", (unsigned long long)(ITER * 2), elapsed);

        uint64_t dt_ns = (t1.tv_sec - t0.tv_sec) * 1000000000ULL + (t1.tv_nsec - t0.tv_nsec);
        // two ops per iteration: multiply and xor
        //printf("%llu,%.6f\n", (unsigned long long)(ITER * 2), dt_ns / 1e9);
        // Note: The above printf uses a fixed-point format for the time in seconds.
        // If you want to print the time in nanoseconds, you can use:
        printf("%llu,%llu\n", (unsigned long long)(ITER * 2), dt_ns);
    }
    return 0;
}
