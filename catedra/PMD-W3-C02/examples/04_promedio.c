#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <omp.h>

int main(int argc, char **argv) {
    long n = 10003; /* No divisible por 2 ni por 4. */
    if (argc > 2) return 2;
    if (argc == 2) {
        char *fin;
        errno = 0;
        n = strtol(argv[1], &fin, 10);
        if (errno || fin == argv[1] || *fin || n < 1 || n > 10000000) {
            fputs("Uso: 04_promedio [N entre 1 y 10000000]\n", stderr);
            return 2;
        }
    }
    double *a = malloc((size_t)n * sizeof(*a));
    if (!a) return 1;
    for (long i = 0; i < n; ++i) a[i] = (double)i;
    //BEGINkernel
    double suma = 0.0;
    #pragma omp parallel for default(none) shared(a,n) reduction(+:suma)
    for (long i = 0; i < n; ++i) {
        suma += a[i];
    }
    double promedio = suma / (double)n;
    //ENDkernel
    double esperado = (n - 1) / 2.0;
    printf("N=%ld promedio=%.6f esperado=%.6f\n", n, promedio, esperado);
    free(a);
    return fabs(promedio - esperado) > 1e-9;
}
