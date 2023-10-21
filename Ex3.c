#include <stdio.h>
#include <stdlib.h>

#define MAX_NODES 8

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

int IsGraphAcyclicUtil(node* nodelist, unsigned vertex, int parent, int* visited);

int IsGraphAcyclic(node* nodelist, unsigned gorder) {
    int visited[MAX_NODES];
    for (unsigned i = 0; i < gorder; i++) {
        visited[i] = 0;
    }

    for (unsigned i = 0; i < gorder; i++) {
        if (!visited[i] && IsGraphAcyclicUtil(nodelist, i, -1, visited)) {
            return 0; // The graph is cyclic.
        }
    }

    return 1; // The graph is acyclic.
}

int IsGraphAcyclicUtil(node* nodelist, unsigned vertex, int parent, int* visited) {
    visited[vertex] = 1;
    for (unsigned i = 0; i < nodelist[vertex].nedges; i++) {
        unsigned neighbor = nodelist[vertex].edges[i];
        if (!visited[neighbor]) {
            if (IsGraphAcyclicUtil(nodelist, neighbor, vertex, visited))
                return 1; // The graph is cyclic.
        } else if (neighbor != parent) {
            return 1; // The graph is cyclic.
        }
    }
    return 0; // The graph is acyclic.S
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

    int connected = IsGraphConnected(nodelist, gorder);
    int acyclic = IsGraphAcyclic(nodelist, gorder);

    if (connected && acyclic) {
        printf("Graph is a tree.\n");
    } else {
        printf("Graph is not a tree.\n");
    }

    free(nodelist); // Clean up memory
}
