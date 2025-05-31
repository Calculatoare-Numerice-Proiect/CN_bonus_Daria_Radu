// #include <stdio.h>
// #include <stdlib.h>
// #include <time.h>
// #include <stdint.h>

// #define STRIDE      64      // bytes
// #define MAX_MB      32      // up to 32 MiB
// #define STEP_MB     4       // step size

// int main() {
//     printf("size_mb,time_ns\n");
//     for (int mb = STEP_MB; mb <= MAX_MB; mb += STEP_MB) {
//         size_t SIZE = (size_t)mb * 1024 * 1024;
//         size_t ITER = SIZE / STRIDE;
//         volatile char *buf = aligned_alloc(64, SIZE);

//         // build pointer-chasing array
//         for (size_t i = 0; i < ITER; i++) {
//             size_t next = ((i+1) % ITER) * STRIDE;
//             ((size_t*)buf)[i] = next;
//         }

//         struct timespec t0, t1;
//         clock_gettime(CLOCK_MONOTONIC, &t0);

//         size_t idx = 0;
//         for (size_t i = 0; i < ITER; i++) {
//             idx = ((size_t*)buf)[idx / STRIDE];
//         }

//         clock_gettime(CLOCK_MONOTONIC, &t1);
//         uint64_t dt_ns = (t1.tv_sec - t0.tv_sec)*1000000000ull
//                        + (t1.tv_nsec - t0.tv_nsec);

//         printf("%d,%llu\n", mb, (unsigned long long)dt_ns);
//         free((void*)buf);
//     }
//     return 0;
// }


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#define STRIDE      64      // one cache line in bytes
#define MAX_MB      64      // maximum buffer size: 32 MiB
#define STEP_MB     8       // step by 4 MiB

// How many times to traverse the entire buffer.
// A single pass is too fast; ramp this up until total_ns > ~1e6 (1 ms).
#define REPEATS 1000

int main() {
    // We will print: size_mb,total_ns
    printf("size_mb,time_ns\n");

    for (int mb = STEP_MB; mb <= MAX_MB; mb += STEP_MB) {
        size_t bytes   = (size_t)mb * 1024 * 1024;
        size_t entries = bytes / STRIDE;
        // We cast to 'volatile' so the compiler cannot optimize away the pointer‐chase
        volatile char *buf = aligned_alloc(STRIDE, bytes);
        if (!buf) {
            fprintf(stderr, "aligned_alloc failed for %zu bytes\n", bytes);
            return 1;
        }

        // Build a pointer-chase loop: each entry points to the next line
        // (wrapping at the end).
        for (size_t i = 0; i < entries; i++) {
            size_t next = ((i + 1) % entries) * STRIDE;
            ((size_t*)buf)[i] = next;
        }

        // Time REPEATS full traversals.
        struct timespec t0, t1;
        clock_gettime(CLOCK_MONOTONIC, &t0);

        for (int r = 0; r < REPEATS; r++) {
            size_t idx = 0;
            // One full pass through 'entries' steps
            for (size_t i = 0; i < entries; i++) {
                idx = ((size_t*)buf)[idx / STRIDE];
            }
        }

        clock_gettime(CLOCK_MONOTONIC, &t1);
        uint64_t total_ns = (uint64_t)(t1.tv_sec  - t0.tv_sec ) * 1000000000ull
                          + (uint64_t)(t1.tv_nsec - t0.tv_nsec);

        // Print the total nanoseconds over REPEATS traversals:
        // e.g.: “4,123456789” means “4 MiB took 123 million ns total”
        printf("%d,%llu\n", mb, (unsigned long long)total_ns);

        free((void*)buf);
    }
    return 0;
}
