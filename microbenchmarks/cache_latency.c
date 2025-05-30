#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#define STRIDE      64               // one cache line in bytes
#define SIZES_COUNT 6
// sizes in KB: 4KB (L1) → 4MB (past L3)
static const size_t SIZES_KB[SIZES_COUNT] = {
    4,    // fits in L1
    32,   // still L1
    256,  // L2
    1024, // L2/L3 boundary
    2048, // L3
    4096  // larger than L3
};

int main() {
    // header: two columns exactly
    printf("size_kb,latency_ns\n");

    for (int s = 0; s < SIZES_COUNT; s++) {
        size_t kb      = SIZES_KB[s];
        size_t bytes   = kb * 1024;
        size_t entries = bytes / STRIDE;
        // prevent optimize-away
        volatile char *buf = aligned_alloc(STRIDE, bytes);

        // build pointer-chase linked structure
        for (size_t i = 0; i < entries; i++) {
            size_t next = ((i + 1) % entries) * STRIDE;
            ((size_t*)buf)[i] = next;
        }

        // time the pointer-chase
        struct timespec t0, t1;
        clock_gettime(CLOCK_MONOTONIC, &t0);
        size_t idx = 0;
        for (size_t i = 0; i < entries; i++) {
            idx = ((size_t*)buf)[idx / STRIDE];
        }
        clock_gettime(CLOCK_MONOTONIC, &t1);

        uint64_t total_ns = (t1.tv_sec  - t0.tv_sec ) * 1000000000ull
                          + (t1.tv_nsec - t0.tv_nsec);
        double avg_ns = (double)total_ns / entries;

        // print average latency per access in ns
        printf("%zu,%.2f\n", kb, avg_ns);

        free((void*)buf);
    }
    return 0;
}
