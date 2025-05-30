#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE (100 * 1024 * 1024)  // 100 MiB

int main() {
    char *buf = aligned_alloc(64, SIZE);
    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);
    memset(buf, 0xFF, SIZE);
    clock_gettime(CLOCK_MONOTONIC, &t1);
    double elapsed = (t1.tv_sec - t0.tv_sec)
                     + (t1.tv_nsec - t0.tv_nsec) * 1e-9;
    printf("bytes=%d time=%.6f\n", SIZE, elapsed);
    free(buf);
    return 0;
}