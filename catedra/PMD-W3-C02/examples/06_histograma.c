#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <omp.h>

_Static_assert(CHAR_BIT == 8, "Este ejemplo requiere bytes de 8 bits");

int main(int argc, char **argv) {
    if (argc > 2) return 2;
    const char *texto = argc == 2 ? argv[1] : "banana bandana";
    size_t n = strlen(texto);
    unsigned long hist[256] = {0};
    //BEGINkernel
    #pragma omp parallel default(none) shared(texto,n,hist)
    {
        unsigned long local[256] = {0};
        #pragma omp for schedule(static)
        for (size_t i = 0; i < n; ++i) {
            unsigned char b = (unsigned char)texto[i];
            local[b]++;
        }
        #pragma omp critical(fusion_histograma)
        {
            for (int b = 0; b < 256; ++b)
                hist[b] += local[b];
        }
    }
    //ENDkernel
    unsigned long referencia[256] = {0};
    for (size_t i = 0; i < n; ++i)
        referencia[(unsigned char)texto[i]]++;
    unsigned long total = 0;
    for (int b = 0; b < 256; ++b) {
        if (hist[b] != referencia[b]) return 1;
        total += hist[b];
        if (hist[b]) printf("byte 0x%02X: %lu\n", b, hist[b]);
    }
    printf("Total=%lu bytes; verificacion=OK\n", total);
    return total != n;
}
