#ifndef COMMON_H
#define COMMON_H
#define _POSIX_C_SOURCE 200809L
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#define MAXV 64
#define INF (LLONG_MAX / 4)
typedef struct { int to, weight; } Edge;
typedef struct { int n, m, degree[MAXV]; Edge adj[MAXV][MAXV]; } Graph;
typedef struct { long long distance; int length, path[MAXV]; } Result;
static Result empty_result(void) { Result r = {.distance=INF,.length=0}; return r; }
//BEGINLOAD
/* First line: vertices edges; then one directed edge per line: from to weight. */
static Graph load_graph(const char *filename) {
    Graph g = {0}; FILE *f = fopen(filename, "r");
    if (!f) { perror(filename); exit(EXIT_FAILURE); }
    if (fscanf(f, "%d%d", &g.n, &g.m) != 2 || g.n < 2 || g.n > MAXV ||
        g.m < 0 || g.m > MAXV * (MAXV - 1)) {
        fputs("Invalid graph header\n", stderr); exit(EXIT_FAILURE);
    }
    for (int i = 0; i < g.m; ++i) {
        int u, v, w;
        if (fscanf(f, "%d%d%d", &u, &v, &w) != 3 ||
            u < 0 || u >= g.n || v < 0 || v >= g.n || u == v ||
            w <= 0 || g.degree[u] >= MAXV) {
            fputs("Invalid edge\n", stderr); exit(EXIT_FAILURE);
        }
        for (int j = 0; j < g.degree[u]; ++j)
            if (g.adj[u][j].to == v) { fputs("Duplicate edge\n", stderr); exit(EXIT_FAILURE); }
        g.adj[u][g.degree[u]++] = (Edge){v, w};
    }
    char extra; if (fscanf(f, " %c", &extra) == 1) { fputs("Extra data\n", stderr); exit(EXIT_FAILURE); }
    fclose(f); return g;
}
//ENDLOAD
static void dfs(const Graph *g, int u, int target, uint64_t visited, long long cost,
                int path[MAXV], int length, Result *best) {
    if (u == target) {
        if (cost < best->distance) {
            best->distance = cost; best->length = length;
            for (int i = 0; i < length; ++i) best->path[i] = path[i];
        }
        return;
    }
    for (int j = 0; j < g->degree[u]; ++j) {
        Edge e = g->adj[u][j]; uint64_t bit = UINT64_C(1) << e.to;
        if (!(visited & bit)) {
            path[length] = e.to;
            dfs(g, e.to, target, visited | bit, cost + e.weight, path, length + 1, best);
        }
    }
}
static Result search_task(const Graph *g, int source, int target, int task) {
    Result r = empty_result(); Edge e = g->adj[source][task];
    int path[MAXV] = {source, e.to};
    dfs(g, e.to, target, (UINT64_C(1) << source) | (UINT64_C(1) << e.to),
        e.weight, path, 2, &r);
    return r;
}
static void combine(Result *best, Result candidate) {
    if (candidate.distance < best->distance) *best = candidate;
}
static double now(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec + t.tv_nsec * 1e-9; }
static int parse_vertex(const char *s, int n) {
    char *end; long v = strtol(s, &end, 10);
    if (!*s || *end || v < 0 || v >= n) { fputs("Invalid vertex\n", stderr); exit(EXIT_FAILURE); }
    return (int)v;
}
static void show(const Graph *g, int source, int target, Result r, double seconds) {
    printf("Vertices: %d; aristas: %d; origen: %d; destino: %d\n", g->n, g->m, source, target);
    if (r.distance == INF) puts("Sin camino");
    else {
        printf("Distancia: %lld; camino:", r.distance);
        for (int i = 0; i < r.length; ++i) printf(" %s%d", i ? "-> " : "", r.path[i]);
        putchar('\n');
    }
    printf("Tiempo de busqueda: %.6f s\n", seconds);
}
static void arguments(int argc, char **argv) {
    if (argc != 4) { fprintf(stderr, "Uso: %s grafo.txt origen destino\n", argv[0]); exit(EXIT_FAILURE); }
}
static Result same_vertex(int source) { Result r = {.distance=0,.length=1,.path={0}}; r.path[0]=source; return r; }
#endif
