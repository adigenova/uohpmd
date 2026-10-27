#include "common.h"
#include <pthread.h>
#define THREADS 4
typedef struct { const Graph *g; int source, target, next; pthread_mutex_t lock; } Queue;
typedef struct { Queue *queue; Result best; } Worker;
//BEGINWORKER
static void *worker(void *arg) {
    Worker *w = arg; Queue *q = w->queue; w->best = empty_result();
    for (;;) {
        pthread_mutex_lock(&q->lock);
        int i = q->next++;
        pthread_mutex_unlock(&q->lock);
        if (i >= q->g->degree[q->source]) break;
        combine(&w->best, search_task(q->g, q->source, q->target, i));
    }
    return NULL;
}
//ENDWORKER
int main(int argc, char **argv) {
    arguments(argc, argv); Graph g = load_graph(argv[1]);
    int source = parse_vertex(argv[2], g.n), target = parse_vertex(argv[3], g.n);
    double start = now(); Result best = empty_result();
    if (source == target) best = same_vertex(source);
    else {
//BEGINTHREADS
        Queue q = {.g=&g, .source=source, .target=target, .next=0,
                   .lock=PTHREAD_MUTEX_INITIALIZER};
        pthread_t threads[THREADS]; Worker w[THREADS];
        for (int t = 0; t < THREADS; ++t) {
            w[t].queue = &q;
            if (pthread_create(&threads[t], NULL, worker, &w[t])) {
                fputs("pthread_create failed\n", stderr); exit(EXIT_FAILURE);
            }
        }
        for (int t = 0; t < THREADS; ++t) {
            if (pthread_join(threads[t], NULL)) { fputs("pthread_join failed\n", stderr); exit(EXIT_FAILURE); }
            combine(&best, w[t].best);
        }
        pthread_mutex_destroy(&q.lock);
//ENDTHREADS
    }
    double elapsed = now() - start;
    show(&g, source, target, best, elapsed); return 0;
}
