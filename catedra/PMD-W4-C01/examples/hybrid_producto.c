#include "common.h"
#include <omp.h>
#define N 1000

int main(int argc, char **argv)
{
    //BEGINsetup
    init_hybrid(&argc, &argv);  // solicita y comprueba FUNNELED
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int begin = (int)((long long)N * rank / size);
    int end = (int)((long long)N * (rank + 1) / size);
    int local_n = end - begin;
    double *a = checked_alloc(local_n * sizeof(*a));
    double *b = checked_alloc(local_n * sizeof(*b));
    #pragma omp parallel for
    for (int i = 0; i < local_n; ++i) {
        a[i] = 1.0;
        b[i] = 2.0;
    }
    //ENDsetup
    //BEGINkernel
    double local_sum = 0.0, global_sum = 0.0;
    #pragma omp parallel for reduction(+:local_sum)
    for (int i = 0; i < local_n; ++i)
        local_sum += a[i] * b[i];
    printf("Proceso %d: producto local = %.1f\n", rank, local_sum);
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE,
               MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0) printf("Producto global = %.1f\n", global_sum);
    free(a);
    free(b);
    MPI_Finalize();
    //ENDkernel
    return 0;
}
