// Author: Sofia Llàcer Caro
// NIA: 1692021

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Define a structure for a node in the queue
typedef struct QueueElementStructure {
    unsigned short vertex;
    struct QueueElementStructure* seg; // Pointer to the next element in the queue
} QueueElement;

// Define the queue structure
typedef struct {
    QueueElement* start; // Pointer to the first element in the queue
    QueueElement* end;   // Pointer to the last element in the queue
} Queue;

// Check if the queue is empty
int IsEmpty(Queue Q) {
    return (Q.start == NULL);
}

// Enqueue an element into the queue
int enqueue(unsigned short vert2Q, Queue* Q) {
    // Create a new queue element
    QueueElement* aux = (QueueElement*)malloc(sizeof(QueueElement));
    if (aux == NULL)
        return 0; // Return 0 if memory allocation fails

    aux->vertex = vert2Q;
    aux->seg = NULL;

    if (Q->start)
        Q->end->seg = aux;
    else
        Q->start = aux;
    Q->end = aux;

    return 1; // Return 1 to indicate successful enqueue
}

// Dequeue an element from the queue
unsigned int dequeue(Queue* Q) {
    if (IsEmpty(*Q))
        return UINT_MAX; // Return a maximum value to indicate an empty queue

    QueueElement* node_inicial = Q->start;
    unsigned int v = node_inicial->vertex;
    Q->start = Q->start->seg;
    free(node_inicial);

    return v;
}

// Define a structure for a graph node
typedef struct {
    unsigned nedges;
    unsigned edges[8];
    int visited;
} node;

// Add this global variable to count connected components
int connectedComponents = 0;

// Function to check if the graph is connected and count connected components using BFS
int isGraphConnected(node* nodelist, unsigned gorder) {
    connectedComponents = 0; // Initialize the connected components counter

    for (unsigned i = 0; i < gorder; i++) {
        if (!nodelist[i].visited) {
            // Start BFS from unvisited nodes to find a connected component
            connectedComponents++;
            Queue q;
            q.start = q.end = NULL;
            nodelist[i].visited = 1;
            enqueue(i, &q);

            while (!IsEmpty(q)) {
                unsigned current = dequeue(&q);
                for (unsigned j = 0; j < nodelist[current].nedges; j++) {
                    unsigned neighbor = nodelist[current].edges[j];
                    if (!nodelist[neighbor].visited) {
                        nodelist[neighbor].visited = 1;
                        enqueue(neighbor, &q);
                    }
                }
            }
        }
    }

    return connectedComponents; // Return the number of connected components
}

int main(int argc, char* argv[]) {
    // Check for the correct number of command line arguments
    if (argc != 2) {
        printf("Usage: %s <graph_file>\n", argv[0]);
        return -1;
    }

    FILE* defgraph;
    node* nodelist;
    unsigned i, j;
    unsigned gsize, gorder, or, dest;

    defgraph = fopen(argv[1], "r");
    if (defgraph == NULL) {
        printf("\nERROR: Data file not found.\n");
        return -1;
    }

    // Read the number of nodes and edges from the file
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

    int connectedComponents = isGraphConnected(nodelist, gorder);
    printf("Number of connected components: %d\n", connectedComponents);

    free(nodelist);
    return 0;
}