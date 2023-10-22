#include <stdio.h>
#include <stdlib.h>

#define MAX_NODES 8

/* 
---------------------
1. Define structures 
---------------------
*/

typedef struct {
    unsigned nedges;
    unsigned edges[MAX_NODES];
    int visited;
} node;

typedef struct {
    unsigned short front;
    unsigned short rear;
    unsigned short size;
    unsigned short capacity;
    unsigned short* array;
} Queue;

/* 
--------------------
2. Define functions 
--------------------
*/

/* 2.1. Queue */

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


/* 2.2. Read file */

int ReadGraphFromFile(const char* fileName, node** nodelist, unsigned* gorder, unsigned* gsize) {
    FILE* defgraph = fopen(fileName, "r");
    if (defgraph == NULL) {
        printf("\nERROR: Data file not found.\n");
        return -1;
    }

    fscanf(defgraph, "%u", gorder);
    fscanf(defgraph, "%u", gsize);

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
        fscanf(defgraph, "%u %u", &or, &dest);
        (*nodelist)[or].edges[(*nodelist)[or].nedges] = dest;
        (*nodelist)[or].nedges++;
        (*nodelist)[dest].edges[(*nodelist)[dest].nedges] = or;
        (*nodelist)[dest].nedges++;
    }

    fclose(defgraph);

    return 0;
}


/* 2.3. Connectedness */

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


/* 2.4. Acyclicity */

int IsGraphAcyclicBFS(node* nodelist, unsigned gorder) {
    int visited[MAX_NODES];
    int parent[MAX_NODES];

    for (unsigned i = 0; i < gorder; i++) {
        visited[i] = 0;
        parent[i] = -1; // Initialize the parent array with -1 (no parent).
    }

    for (unsigned i = 0; i < gorder; i++) {
        if (!visited[i]) {
            Queue queue;
            queue.front = queue.rear = 0;
            queue.size = 0;
            queue.capacity = gorder;
            queue.array = (unsigned short*)malloc(gorder * sizeof(unsigned short));

            visited[i] = 1;
            EnqueueGraphNode(i, &queue);

            while (!IsGraphQueueEmpty(&queue)) {
                unsigned currentNode = DequeueGraphNode(&queue);
                for (unsigned j = 0; j < nodelist[currentNode].nedges; j++) {
                    unsigned neighbor = nodelist[currentNode].edges[j];
                    if (!visited[neighbor]) {
                        visited[neighbor] = 1;
                        parent[neighbor] = currentNode; // Set the parent of the neighbor.
                        EnqueueGraphNode(neighbor, &queue);
                    } else if (neighbor != parent[currentNode]) {
                        free(queue.array);
                        return 0; // The graph is cyclic.
                    }
                }
            }

            free(queue.array);
        }
    }

    return 1; // The graph is acyclic.
}


/* 
------------------------------------
3. Command-line Argument Processing 
------------------------------------
*/

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

    // Boolean expression for the tree requirements

    int connected = IsGraphConnected(nodelist, gorder);
    int acyclic = IsGraphAcyclicBFS(nodelist, gorder);

    if (connected && acyclic) {
        printf("Graph is a tree.\n");
    } else {
        printf("Graph is not a tree.\n");
    }

    free(nodelist); // Clearn up memory
}
