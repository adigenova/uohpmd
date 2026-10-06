#include <stdio.h>
#include <omp.h>

int main(void) {
    enum { N = 48 };
    double a[N][N], b[N][N], c[N][N];
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j) {
            a[i][j] = i + j;
            b[i][j] = i == j ? 2.0 : 0.0;
        }
    //BEGINkernel
    #pragma omp parallel for collapse(2) default(none) \
        shared(a,b,c) schedule(static)
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            double suma = 0.0;
            for (int k = 0; k < N; ++k)
                suma += a[i][k] * b[k][j];
            c[i][j] = suma;
        }
    }
    //ENDkernel
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j) {
            double ref = 0.0;
            for (int k = 0; k < N; ++k) ref += a[i][k] * b[k][j];
            if (c[i][j] != ref || c[i][j] != 2.0 * (i + j)) return 1;
        }
    printf("C[0][0]=%.0f C[47][47]=%.0f verificacion=OK\n", c[0][0], c[47][47]);
    return 0;
}
