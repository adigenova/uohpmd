#include "common.h"

int main(int argc, char **argv)
{
    //BEGINsetup
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    const int N = 1000;
    if (N % size != 0) {
        if (rank == 0) fprintf(stderr, "N debe ser divisible por P.\n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        return EXIT_FAILURE;
    }
    int local_n = N / size;
    int *data = NULL;
    int *local = checked_alloc(local_n * sizeof(*local));
    if (rank == 0) {
        data = checked_alloc(N * sizeof(*data));
        for (int i = 0; i < N; ++i) data[i] = i;
    }
    //ENDsetup
    //BEGINkernel
    MPI_Scatter(data, local_n, MPI_INT,
                local, local_n, MPI_INT, 0, MPI_COMM_WORLD);
    int local_sum = 0, global_sum = 0;
    for (int i = 0; i < local_n; ++i) local_sum += local[i];
    printf("Proceso %d: suma local = %d\n", rank, local_sum);
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT,
               MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0) printf("Suma global = %d\n", global_sum);
    //ENDkernel
    free(local);
    free(data);
    MPI_Finalize();
    return 0;
}
