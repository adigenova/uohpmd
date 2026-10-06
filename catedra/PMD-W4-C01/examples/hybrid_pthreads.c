#include "common.h"
#include <pthread.h>
#define N 1000000
#define THREADS 4

//BEGINworker
struct ThreadData {
    const int *array;
    int begin, end;
    long long sum;
};

static void *calculate_sum(void *arg)
{
    struct ThreadData *d = arg;
    d->sum = 0;
    for (int i = d->begin; i < d->end; ++i)
        d->sum += d->array[i];
    return NULL;
}
//ENDworker

int main(int argc, char **argv)
{
    init_hybrid(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    //BEGINsetup
    int first = (int)((long long)N * rank / size);
    int end = (int)((long long)N * (rank + 1) / size);
    int local_n = end - first;
    int *data = checked_alloc(local_n * sizeof(*data));
    for (int i = 0; i < local_n; ++i) data[i] = (first + i) % 10;
    pthread_t threads[THREADS];
    struct ThreadData work[THREADS];
    //ENDsetup
    //BEGINlaunch
    for (int t = 0; t < THREADS; ++t) {
        work[t].array = data;
        work[t].begin = (int)((long long)local_n * t / THREADS);
        work[t].end = (int)((long long)local_n * (t + 1) / THREADS);
        work[t].sum = 0;
        int error = pthread_create(&threads[t], NULL,
                                   calculate_sum, &work[t]);
        if (error) MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
    //ENDlaunch
    //BEGINreduce
    long long local_sum = 0, global_sum = 0;
    for (int t = 0; t < THREADS; ++t) {
        if (pthread_join(threads[t], NULL))
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        local_sum += work[t].sum;
    }
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_LONG_LONG_INT,
               MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0) printf("Suma global = %lld\n", global_sum);
    free(data);
    MPI_Finalize();
    //ENDreduce
    return 0;
}
