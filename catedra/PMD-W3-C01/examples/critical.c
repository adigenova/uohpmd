#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int sum = 0, contribution[10];
    /* Generar los valores en serie. */
    for (int k = 0; k < 10; k++)
        contribution[k] = rand() % 50;

    #pragma omp parallel for shared(sum, contribution)
    for (int k = 0; k < 10; k++) {
        int c = contribution[k];
        printf("Itr: %d tid=%d, my_contri=%d\n",
               k, omp_get_thread_num(), c);
        #pragma omp critical
        { sum += c; }
    }
    printf("Sum=%d\n", sum);
    return 0;
}
