#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#define STRIDE      64               // bytes per cache line
#define SIZES_COUNT 6
#define REPEATS     10000            // number of full traversals per size

// sizes in KB: 4KB (L1) → 4MB (past L3)
static const size_t SIZES_KB[SIZES_COUNT] = { 4,32,256,1024,2048,4096 };

int main() {
    printf("size_kb,latency_ns\n");

    for (int s = 0; s < SIZES_COUNT; s++) {
        size_t kb      = SIZES_KB[s];
        size_t bytes   = kb * 1024;
        size_t entries = bytes / STRIDE;
        volatile char *buf = aligned_alloc(STRIDE, bytes);

        // build the pointer‐chase list
        for (size_t i = 0; i < entries; i++) {
            size_t next = ((i + 1) % entries) * STRIDE;
            ((size_t*)buf)[i] = next;
        }

        // time REPEATS full traversals
        struct timespec t0, t1;
        clock_gettime(CLOCK_MONOTONIC, &t0);
        for (int r = 0; r < REPEATS; r++) {
            size_t idx = 0;
            for (size_t i = 0; i < entries; i++) {
                idx = ((size_t*)buf)[idx / STRIDE];
            }
        }
        clock_gettime(CLOCK_MONOTONIC, &t1);

        uint64_t total_ns = (t1.tv_sec  - t0.tv_sec ) * 1000000000ull
                          + (t1.tv_nsec - t0.tv_nsec);
        // average per access over all REPEATS traversals
        double avg_ns = (double)total_ns / (entries * REPEATS);

        // print with enough precision
        printf("%zu,%.3f\n", kb, avg_ns);

        free((void*)buf);
    }
    return 0;
}
