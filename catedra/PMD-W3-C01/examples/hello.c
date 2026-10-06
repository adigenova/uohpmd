#include <omp.h>
#include <stdio.h>

int main(void) {
    #pragma omp parallel
    {
        printf("Hola Mundo... desde hilo = %d\n",
               omp_get_thread_num());
    }
    return 0;
}
