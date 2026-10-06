#include "common.h"
#include <omp.h>

int main(int argc, char **argv)
{
    //BEGINinit
    int requested = MPI_THREAD_FUNNELED, provided;
    MPI_Init_thread(&argc, &argv, requested, &provided);
    if (provided < requested) {
        fprintf(stderr, "Soporte insuficiente: %d < %d\n",
                provided, requested);
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        return EXIT_FAILURE;
    }
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    //ENDinit
    //BEGINhello
    printf("Hola desde proceso %d de %d\n", rank, size);
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int nt = omp_get_num_threads();
        printf("MPI rank %d: OpenMP hilo %d de %d\n", rank, tid, nt);
    }
    MPI_Finalize();
    //ENDhello
    return 0;
}
