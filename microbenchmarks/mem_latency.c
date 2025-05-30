#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE (8 * 1024 * 1024)  // 8 MiB
#define STRIDE 64              // one cache line
#define ITER (SIZE / STRIDE)

int main() {
    char *buf = aligned_alloc(64, SIZE);
    // build a pointer-chasing array
    for (int i = 0; i < SIZE/STRIDE; i++) {
        size_t idx = ((i+1) % (SIZE/STRIDE)) * STRIDE;
        ((size_t*)buf)[i] = idx;
    }
    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);
    size_t idx = 0;
    for (int i = 0; i < ITER; i++) {
        idx = ((size_t*)buf)[idx/STRIDE];
    }
    clock_gettime(CLOCK_MONOTONIC, &t1);
    double elapsed = (t1.tv_sec - t0.tv_sec)
                     + (t1.tv_nsec - t0.tv_nsec) * 1e-9;
    //printf("accesses=%d time=%.6f\n", ITER, elapsed);
    printf("accesses,time_s\n");
    printf("%d,%.6f\n", ITER, elapsed);
    free(buf);
    return 0;
}