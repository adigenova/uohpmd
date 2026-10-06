#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size != 2) {
        if (rank == 0) fputs("Ejecutar con exactamente 2 procesos\n", stderr);
        MPI_Finalize();
        return 2;
    }
    //BEGINkernel
    int valor = 0;
    if (rank == 0) {
        valor = 42;
        MPI_Send(&valor, 1, MPI_INT, 1, 7, MPI_COMM_WORLD);
    } else if (rank == 1) {
        MPI_Recv(&valor, 1, MPI_INT, 0, 7,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Rank 1 recibio %d\n", valor);
    }
    //ENDkernel
    MPI_Finalize();
    return valor != 42;
}
