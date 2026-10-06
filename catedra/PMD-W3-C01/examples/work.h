#ifndef WORK_H
#define WORK_H
#include <omp.h>
#include <stdio.h>
#include <unistd.h>

static inline void Work1(void) {
    printf("executing work 1 hilo:%d\n", omp_get_thread_num());
    sleep(1);
}
static inline void Work2(void) {
    printf("executing work 2 hilo:%d\n", omp_get_thread_num());
    sleep(1);
}
static inline void Work3(void) {
    printf("executing work 3 hilo:%d\n", omp_get_thread_num());
}
static inline void Work4(void) {
    printf("executing work 4 hilo:%d\n", omp_get_thread_num());
    sleep(1);
}
#endif
