#include "common.h"
#include <math.h>

int main(int argc, char **argv)
{
    //BEGINsetup
    MPI_Init(&argc, &argv);
    int rank, size, n = 5000;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    double h = 1.0 / n, sum = 0.0;
    for (int i = rank + 1; i <= n; i += size) {
        double x = h * (i - 0.5);
        sum += 4.0 / (1.0 + x * x);
    }
    double local_pi = h * sum;
    //ENDsetup
    //BEGINreduce
    double pi = 0.0;
    MPI_Reduce(&local_pi, &pi, 1, MPI_DOUBLE,
               MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        const double reference = 3.14159265358979323846;
        printf("pi = %.16f; error = %.16f\n",
               pi, fabs(pi - reference));
    }
    MPI_Finalize();
    //ENDreduce
    return 0;
}
