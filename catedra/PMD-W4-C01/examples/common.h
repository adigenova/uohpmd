#ifndef PMD_COMMON_H
#define PMD_COMMON_H
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

static inline void *checked_alloc(size_t bytes)
{
    void *p = malloc(bytes ? bytes : 1);
    if (!p) {
        fprintf(stderr, "No se pudo reservar memoria.\n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        exit(EXIT_FAILURE);
    }
    return p;
}

static inline void init_hybrid(int *argc, char ***argv)
{
    int provided;
    MPI_Init_thread(argc, argv, MPI_THREAD_FUNNELED, &provided);
    if (provided < MPI_THREAD_FUNNELED) {
        fprintf(stderr, "MPI no ofrece MPI_THREAD_FUNNELED.\n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        exit(EXIT_FAILURE);
    }
}
#endif
