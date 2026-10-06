#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    //BEGINkernel
    int n = 10, local = 0, total = 0;
    /* Distribucion ciclica: cubre todos los elementos, aun si n % size != 0. */
    for (int i = rank; i < n; i += size)
        local += i + 1;
    MPI_Reduce(&local, &total, 1, MPI_INT, MPI_SUM,
               0, MPI_COMM_WORLD);
    if (rank == 0) printf("Suma global = %d\n", total);
    //ENDkernel
    int error = rank == 0 && total != 55;
    MPI_Finalize();
    return error;
}
