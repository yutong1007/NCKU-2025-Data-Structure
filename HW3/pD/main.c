#include "mst.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int u, v, w, edge_count = 0;
    Node *node_u, *node_v;
    int *discovered = (int *)malloc(sizeof(int) * MAX_NODES);
    Node **nodes = (Node **)malloc(sizeof(Node *) * MAX_NODES);
    Edge **edges = (Edge **)malloc(sizeof(Edge *) * MAX_EDGES);

    memset(discovered, 0, sizeof(int) * MAX_NODES);
    memset(nodes, 0, sizeof(Node *) * MAX_NODES);
    memset(edges, 0, sizeof(Edge *) * MAX_EDGES);

    while (scanf("%d%d%d", &u, &v, &w) == 3) {
        node_u = nodes[u];
        if (node_u == NULL) {
            node_u = (Node *)malloc(sizeof(Node));
            node_u->id = u;
            node_u->edge_count = 0;
            node_u->edges = NULL;
            nodes[u] = node_u;

            node_u->var = NULL;
        }
        node_v = nodes[v];
        if (node_v == NULL) {
            node_v = (Node *)malloc(sizeof(Node));
            node_v->id = v;
            node_v->edge_count = 0;
            node_v->edges = NULL;
            nodes[v] = node_v;

            node_v->var = NULL;
        }

        node_u->edge_count++;
        node_v->edge_count++;

        Edge *new_edge = (Edge *)malloc(sizeof(Edge));
        new_edge->u = node_u;
        new_edge->v = node_v;
        new_edge->w = w;
        new_edge->keep = 0;
        new_edge->var = NULL;

        edges[edge_count++] = new_edge;
    }

    for (int i = 0; i < edge_count; i++) {
        Edge *edge = edges[i];
        node_u = edge->u;
        node_v = edge->v;

        if (node_u->edges == NULL) {
            node_u->edges =
                (Edge **)malloc(sizeof(Edge *) * node_u->edge_count);
            node_u->edge_count = 0;
        }
        node_u->edges[node_u->edge_count++] = edge;

        if (node_v->edges == NULL) {
            node_v->edges =
                (Edge **)malloc(sizeof(Edge *) * node_v->edge_count);
            node_v->edge_count = 0;
        }
        node_v->edges[node_v->edge_count++] = edge;
    }

    generate_mst(nodes[0]);

    int node_count = 0;
    int keep_edge_count = 0;
    unsigned long long total_weight = 0;
    for (int i = 0; i < edge_count; i++) {
        Edge *edge = edges[i];
        if (edge->keep) {
            total_weight += edge->w;
            keep_edge_count++;

            u = edge->u->id;
            v = edge->v->id;
            if (!discovered[u]) {
                discovered[u] = 1;
                node_count++;
            }
            if (!discovered[v]) {
                discovered[v] = 1;
                node_count++;
            }
        }
    }
    printf("%d %d %llu\n", node_count, keep_edge_count, total_weight);
}