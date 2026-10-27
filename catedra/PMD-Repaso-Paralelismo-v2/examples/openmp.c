#include "common.h"
#include <omp.h>
int main(int argc, char **argv) {
    arguments(argc, argv); Graph g = load_graph(argv[1]);
    int source = parse_vertex(argv[2], g.n), target = parse_vertex(argv[3], g.n);
    double start = now(); Result best = empty_result();
    if (source == target) best = same_vertex(source);
    else {
//BEGINOPENMP
        Result results[MAXV];
        #pragma omp parallel for schedule(dynamic, 1)
        for (int i = 0; i < g.degree[source]; ++i)
            results[i] = search_task(&g, source, target, i);
        for (int i = 0; i < g.degree[source]; ++i)
            combine(&best, results[i]);
//ENDOPENMP
    }
    double elapsed = now() - start;
    show(&g, source, target, best, elapsed); return 0;
}
