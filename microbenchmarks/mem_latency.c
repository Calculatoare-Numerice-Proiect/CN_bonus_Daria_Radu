#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define STRIDE        64    // one cache line
#define MAX_SIZE_MB   32    // up to 32 MiB
#define STEP_MB       4     // in 4 MiB increments

int main() {
    printf("size_mb,accesses,time_s\n");
    for (int size = STEP_MB; size <= MAX_SIZE_MB; size += STEP_MB) {
        size_t SIZE = (size * 1024 * 1024);
        size_t ITER = SIZE / STRIDE;
        char *buf = aligned_alloc(64, SIZE);

        // build pointer chase
        for (size_t i = 0; i < ITER; i++) {
            size_t idx = ((i+1) % ITER) * STRIDE;
            ((size_t*)buf)[i] = idx;
        }

        struct timespec t0, t1;
        clock_gettime(CLOCK_MONOTONIC, &t0);
        size_t idx = 0;
        for (size_t i = 0; i < ITER; i++) {
            idx = ((size_t*)buf)[idx/STRIDE];
        }
        clock_gettime(CLOCK_MONOTONIC, &t1);

        double elapsed = (t1.tv_sec - t0.tv_sec)
                       + (t1.tv_nsec - t0.tv_nsec) * 1e-9;
        printf("%d, %zu, %.6f\n", size, ITER, elapsed);
        free(buf);
    }
    return 0;
}
