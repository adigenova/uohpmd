#include <stdio.h>
#include <omp.h>

int main(void) {
    #pragma omp parallel default(none)
    {
        int id = omp_get_thread_num();
        int equipo = omp_get_num_threads();
        printf("Hola desde hilo %d de %d\n", id, equipo);
    }
    puts("Fin de la region paralela");
    return 0;
}
