#include <stdio.h>
#include <omp.h>

int main(void) {
    //BEGINkernel
    int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int n = 10;
    int suma = 0;
    #pragma omp parallel for default(none) shared(a,n) reduction(+:suma)
    for (int i = 0; i < n; ++i) {
        suma += a[i];
    }
    printf("Suma = %d\n", suma);
    //ENDkernel
    return suma != 55;
}
