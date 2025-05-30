#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE_MB   256   // up to 256 MiB
#define STEP_MB       32    // in 32 MiB increments

int main() {
    printf("size_mb,bytes,time_s\n");
    for (int size = STEP_MB; size <= MAX_SIZE_MB; size += STEP_MB) {
        size_t SIZE = size * 1024 * 1024;
        char *buf = aligned_alloc(64, SIZE);

        struct timespec t0, t1;
        clock_gettime(CLOCK_MONOTONIC, &t0);
        memset(buf, 0xFF, SIZE);
        clock_gettime(CLOCK_MONOTONIC, &t1);

        double elapsed = (t1.tv_sec - t0.tv_sec)
                       + (t1.tv_nsec - t0.tv_nsec) * 1e-9;
        printf("%d, %zu, %.6f\n", size, SIZE, elapsed);
        free(buf);
    }
    return 0;
}
