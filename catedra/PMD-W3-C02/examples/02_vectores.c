#include <stdio.h>
#include <omp.h>

int main(void) {
    enum { N = 12 };
    int a[N], b[N], c[N];
    for (int i = 0; i < N; ++i) {
        a[i] = i;
        b[i] = 2 * i;
    }
    //BEGINkernel
    #pragma omp parallel for default(none) shared(a,b,c) schedule(static)
    for (int i = 0; i < N; ++i) {
        c[i] = a[i] + b[i];
    }
    //ENDkernel
    for (int i = 0; i < N; ++i) {
        if (c[i] != 3 * i) return 1;
        printf("%d%s", c[i], i == N - 1 ? "\n" : " ");
    }
    return 0;
}
