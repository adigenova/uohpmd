#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <omp.h>

enum { UMBRAL = 2048 };

static void intercambiar(int *a, int *b) {
    int t = *a; *a = *b; *b = t;
}

/* Particion de Hoare: [lo,p] y [p+1,hi], ambos inclusive. */
static int particionar(int *a, int lo, int hi) {
    int pivote = a[lo + (hi - lo) / 2];
    int i = lo - 1, j = hi + 1;
    for (;;) {
        do { ++i; } while (a[i] < pivote);
        do { --j; } while (a[j] > pivote);
        if (i >= j) return j;
        intercambiar(&a[i], &a[j]);
    }
}

static int comparar(const void *a, const void *b) {
    int x = *(const int *)a, y = *(const int *)b;
    return (x > y) - (x < y);
}

//BEGINtareas
static void quicksort(int *a, int lo, int hi) {
    if (lo >= hi) return;
    if (hi - lo + 1 <= UMBRAL) {
        qsort(a + lo, (size_t)(hi - lo + 1), sizeof(*a), comparar);
        return;
    }
    int p = particionar(a, lo, hi);
    #pragma omp task default(none) firstprivate(a,lo,p)
    { quicksort(a, lo, p); }
    #pragma omp task default(none) firstprivate(a,p,hi)
    { quicksort(a, p + 1, hi); }
    #pragma omp taskwait
}
//ENDtareas

int main(int argc, char **argv) {
    int n = 100000;
    const char *modo = argc > 2 ? argv[2] : "aleatorio";
    if (argc > 3) return 2;
    if (argc > 1) {
        char *fin;
        errno = 0;
        long valor = strtol(argv[1], &fin, 10);
        if (errno || fin == argv[1] || *fin || valor < 1 || valor > 10000000)
            return 2;
        n = (int)valor;
    }
    if (strcmp(modo, "aleatorio") && strcmp(modo, "ordenado") &&
        strcmp(modo, "inverso") && strcmp(modo, "iguales")) return 2;
    int *a = malloc((size_t)n * sizeof(*a));
    int *ref = malloc((size_t)n * sizeof(*ref));
    if (!a || !ref) { free(a); free(ref); return 1; }
    uint32_t estado = 42;
    for (int i = 0; i < n; ++i) {
        estado = UINT32_C(1664525) * estado + UINT32_C(1013904223);
        if (!strcmp(modo, "ordenado")) a[i] = i;
        else if (!strcmp(modo, "inverso")) a[i] = n - i;
        else if (!strcmp(modo, "iguales")) a[i] = 7;
        else a[i] = (int)(estado % 10000);
    }
    memcpy(ref, a, (size_t)n * sizeof(*ref));
    qsort(ref, (size_t)n, sizeof(*ref), comparar);
    double t0 = omp_get_wtime();
    //BEGINlanzamiento
    #pragma omp parallel default(none) shared(a,n)
    {
        #pragma omp single
        { quicksort(a, 0, n - 1); }
    }
    //ENDlanzamiento
    double tiempo = omp_get_wtime() - t0;
    int ok = memcmp(a, ref, (size_t)n * sizeof(*a)) == 0;
    printf("N=%d modo=%s ordenado=%s tiempo=%.6f\n", n, modo, ok ? "OK" : "ERROR", tiempo);
    free(ref);
    free(a);
    return !ok;
}
