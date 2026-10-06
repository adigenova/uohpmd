#include "common.h"

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size < 2) {
        if (rank == 0) fprintf(stderr, "Se necesitan al menos 2 procesos.\n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        return EXIT_FAILURE;
    }
    //BEGINdatatype
    int point[3];  // x, y, z contiguos en memoria
    MPI_Datatype point_type;
    MPI_Type_contiguous(3, MPI_INT, &point_type);
    MPI_Type_commit(&point_type);
    //ENDdatatype
    //BEGINtransfer
    if (rank == 1) {
        point[0] = 45; point[1] = 36; point[2] = 0;
        MPI_Send(point, 1, point_type, 0, 0, MPI_COMM_WORLD);
    }
    if (rank == 0) {
        MPI_Recv(point, 1, point_type, 1, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Punto recibido: (%d, %d, %d)\n",
               point[0], point[1], point[2]);
    }
    MPI_Type_free(&point_type);
    MPI_Finalize();
    //ENDtransfer
    return 0;
}
