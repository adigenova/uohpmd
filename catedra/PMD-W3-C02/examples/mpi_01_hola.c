#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, size, longitud;
    char nombre[MPI_MAX_PROCESSOR_NAME];
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Get_processor_name(nombre, &longitud);
    printf("Hola desde rank %d de %d en %.*s\n",
           rank, size, longitud, nombre);
    MPI_Finalize();
    return 0;
}
