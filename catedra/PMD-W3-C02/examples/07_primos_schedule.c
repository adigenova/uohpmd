#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <omp.h>

//BEGINprimalidad
static int es_primo(int x) {
    if (x < 2) return 0;
    for (int d = 2; d <= x / d; ++d)
        if (x % d == 0) return 0;
    return 1;
}
//ENDprimalidad

int main(int argc, char **argv) {
    int n = 200000;
    if (argc > 2) return 2;
    if (argc == 2) {
        char *fin;
        errno = 0;
        long valor = strtol(argv[1], &fin, 10);
        if (errno || fin == argv[1] || *fin || valor < 2 || valor > 10000000)
            return 2;
        n = (int)valor;
    }
    int referencia = 0;
    double t0 = omp_get_wtime();
    for (int x = 2; x <= n; ++x) referencia += es_primo(x);
    double t_serial = omp_get_wtime() - t0;
    //BEGINkernel
    int cuenta = 0;
    t0 = omp_get_wtime();
    #pragma omp parallel for default(none) shared(n) \
        reduction(+:cuenta) schedule(runtime)
    for (int x = 2; x <= n; ++x) {
        cuenta += es_primo(x);
    }
    double t_paralelo = omp_get_wtime() - t0;
    //ENDkernel
    printf("N=%d primos=%d referencia=%d\n", n, cuenta, referencia);
    printf("T_serial=%.6f T_paralelo=%.6f segundos\n", t_serial, t_paralelo);
    return cuenta != referencia;
}
