#include <stdio.h>
#include <omp.h>

int main(void) {
    int a[] = {4, 1, 7, 3, 9, 2};
    int n = 6;
    int suma = 0, maximo = a[0];
    //BEGINkernel
    #pragma omp parallel sections default(none) shared(a,n,suma,maximo)
    {
        #pragma omp section
        {
            for (int i = 0; i < n; ++i) suma += a[i];
        }
        #pragma omp section
        {
            for (int i = 1; i < n; ++i)
                if (a[i] > maximo) maximo = a[i];
        }
    }
    //ENDkernel
    printf("Suma=%d maximo=%d\n", suma, maximo);
    return suma != 26 || maximo != 9;
}
