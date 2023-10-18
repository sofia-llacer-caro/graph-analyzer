// Author: Sofia Llàcer Caro
// NIA: 1692021

#include <stdio.h>
#include <stdlib.h>

// Define the node structure
typedef struct {
    unsigned nedges;
    unsigned edges[8]; // Limit of edges in a node = 8
    int visited;
} node;

// Define the queue structure
typedef struct {
    unsigned short front;
    unsigned short rear;
    unsigned short size;
    unsigned short capacity;
    unsigned short* array;
} Queue;

/* Define functions */

/* Queue */
int IsGraphQueueEmpty(Queue* Q) {
    return (Q->size == 0);
}

int EnqueueGraphNode(unsigned short nodeValue, Queue* Q) {
    if (Q->size == Q->capacity)
        return 0; // Queue is full

    Q->array[Q->rear] = nodeValue;
    Q->rear = (Q->rear + 1) % Q->capacity;
    Q->size++;

    return 1;
}

unsigned int DequeueGraphNode(Queue* Q) {
    if (IsGraphQueueEmpty(Q))
        return (unsigned int)-1; // Indicates an error condition

    unsigned int nodeValue = Q->array[Q->front];
    Q->front = (Q->front + 1) % Q->capacity;
    Q->size--;

    return nodeValue;
}

/* Read file */
int ReadGraphFromFile(const char* fileName, node** nodelist, unsigned* gorder, unsigned* gsize) {
    FILE* graphFile = fopen(fileName, "r");
    if (graphFile == NULL) {
        printf("\nERROR: Data file not found.\n");
        return -1;
    }

    fscanf(graphFile, "%u", gorder);
    fscanf(graphFile, "%u", gsize);

    if ((*nodelist = (node*)malloc(*gorder * sizeof(node))) == NULL) {
        fprintf(stderr, "\nERROR: Not enough memory for allocating the nodes\n\n");
        return 2;
    }

    for (unsigned i = 0; i < *gorder; i++) {
        (*nodelist)[i].nedges = 0;
        (*nodelist)[i].visited = 0;
    }

    for (unsigned j = 0; j < *gsize; j++) {
        unsigned or, dest;
        fscanf(graphFile, "%u %u", &or, &dest);
        (*nodelist)[or].edges[(*nodelist)[or].nedges] = dest;
        (*nodelist)[or].nedges++;
        (*nodelist)[dest].edges[(*nodelist)[dest].nedges] = or;
        (*nodelist)[dest].nedges++;
    }

    fclose(graphFile);
    return 0;
}

/* Connectedness */
int IsGraphConnected(node* nodelist, unsigned gorder) {
    Queue queue;
    queue.front = queue.rear = 0;
    queue.size = 0;
    queue.capacity = gorder;
    queue.array = (unsigned short*)malloc(gorder * sizeof(unsigned short));

    nodelist[0].visited = 1;
    EnqueueGraphNode(0, &queue);

    while (!IsGraphQueueEmpty(&queue)) {
        unsigned currentNode = DequeueGraphNode(&queue);
        for (unsigned i = 0; i < nodelist[currentNode].nedges; i++) {
            unsigned neighbor = nodelist[currentNode].edges[i];
            if (!nodelist[neighbor].visited) {
                nodelist[neighbor].visited = 1;
                EnqueueGraphNode(neighbor, &queue);
            }
        }
    }

    for (unsigned i = 0; i < gorder; i++) {
        if (nodelist[i].visited == 0) {
            free(queue.array);
            return 0;
        }
    }

    free(queue.array);
    return 1;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <graph_file>\n", argv[0]);
        return -1;
    }

    node* nodelist;
    unsigned gorder, gsize;

    if (ReadGraphFromFile(argv[1], &nodelist, &gorder, &gsize) != 0) {
        return -1;
    }

    printf("%s defines a graph with %u nodes and %u edges.\n", argv[1], gorder, gsize);

    int isConnected = IsGraphConnected(nodelist, gorder);
    if (isConnected) {
        printf("The graph is connected.\n");
    } else {
        printf("The graph is not connected.\n");
    }

    // Print node information
    for (unsigned i = 0; i < gorder; i++) {
        printf("Node %u has %u edges:\n", i, nodelist[i].nedges);
        for (unsigned j = 0; j < nodelist[i].nedges; j++) {
            printf("  %u --> %u\n", i, nodelist[i].edges[j]);
        }
    }

    // Clean up memory
    free(nodelist);

    return isConnected;
}
