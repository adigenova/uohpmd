#include "common.h"
#define N 8

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (N % size != 0) {
        if (rank == 0) fprintf(stderr, "N debe ser divisible por P.\n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        return EXIT_FAILURE;
    }
    //BEGINsetup
    int a[N * N], b[N * N];
    int rows = N / size, first = rank * rows;
    int *local = checked_alloc(rows * N * sizeof(*local));
    int *c = rank == 0 ? checked_alloc(N * N * sizeof(*c)) : NULL;
    if (rank == 0) {
        for (int i = 0; i < N * N; ++i) {
            a[i] = 1;
            b[i] = 2;
        }
    }
    MPI_Bcast(a, N * N, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(b, N * N, MPI_INT, 0, MPI_COMM_WORLD);
    //ENDsetup
    //BEGINkernel
    for (int r = 0; r < rows; ++r) {
        for (int j = 0; j < N; ++j) {
            int value = 0;
            for (int k = 0; k < N; ++k)
                value += a[(first + r) * N + k] * b[k * N + j];
            local[r * N + j] = value;
        }
    }
    MPI_Gather(local, rows * N, MPI_INT,
               c, rows * N, MPI_INT, 0, MPI_COMM_WORLD);
    //ENDkernel
    if (rank == 0) {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) printf("%d ", c[i * N + j]);
            putchar('\n');
        }
    }
    free(local);
    free(c);
    MPI_Finalize();
    return 0;
}
