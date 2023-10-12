// Author: Sofia Llàcer Caro
// NIA: 1692021

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    unsigned nedges;
    unsigned edges[8]; // limit of edges in a node = 8
    int visited; // New field to track visited nodes
} node;

void DFS(node* nodelist, unsigned node_idx, unsigned gorder) {
    nodelist[node_idx].visited = 1;
    for (unsigned i = 0; i < nodelist[node_idx].nedges; i++) {
        unsigned neighbor = nodelist[node_idx].edges[i];
        if (!nodelist[neighbor].visited) {
            DFS(nodelist, neighbor, gorder);
        }
    }
}

int main(int argc, char* argv[]) {
    FILE* defgraph;
    node* nodelist;
    unsigned i, j;
    unsigned gsize, gorder, or, dest;

    defgraph = fopen(argv[1], "r");
    if (defgraph == NULL) {
        printf("\nERROR: Data file not found.\n");
        return -1;
    }
    fscanf(defgraph, "%u", &gorder);
    fscanf(defgraph, "%u", &gsize);
    if ((nodelist = (node*)malloc(gorder * sizeof(node))) == NULL) {
        fprintf(stderr, "\nERROR: not enough memory for allocating the nodes\n\n");
        return 2;
    }
    printf("%s defines a graph with %d nodes and %d edges.\n", argv[1], gorder, gsize);
    for (i = 0; i < gorder; i++) {
        nodelist[i].nedges = 0;
        nodelist[i].visited = 0;
    }
    for (j = 0; j < gsize; j++) {
        fscanf(defgraph, "%u %u", &or, &dest);
        nodelist[or].edges[nodelist[or].nedges] = dest;
        nodelist[or].nedges++;
        nodelist[dest].edges[nodelist[dest].nedges] = or;
        nodelist[dest].nedges++;
    }
    fclose(defgraph);

    // Start DFS from the first node
    DFS(nodelist, 0, gorder);

    // Check if all nodes were visited
    for (i = 0; i < gorder; i++) {
        if (nodelist[i].visited == 0) {
            // The graph is not connected
            printf("The graph is not connected.\n");
            free(nodelist);
            return 0;
        }
    }

    // The graph is connected
    printf("The graph is connected.\n");
    free(nodelist);
    return 1;
}