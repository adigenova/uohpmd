#include <omp.h>
#include <stdio.h>

int main(void) {
    #pragma omp parallel
    {
        #pragma omp for schedule(static)
        for (int k = 0; k < 10; k++) {
            printf("Itr: %d tid=%d\n", k,
                   omp_get_thread_num());
        }
    }
    return 0;
}
