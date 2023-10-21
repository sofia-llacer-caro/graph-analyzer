#include <stdio.h>
#include <stdlib.h>

/* 
---------------------
1. Define structures 
---------------------
*/

typedef struct {
    unsigned nedges;
    unsigned edges[8];
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

int CountConnectedComponents(node* nodelist, unsigned gorder) {
    int components = 0;
    for (unsigned i = 0; i < gorder; i++) {
        if (!nodelist[i].visited) {
            components++;
            Queue queue;
            queue.front = queue.rear = 0;
            queue.size = 0;
            queue.capacity = gorder;
            queue.array = (unsigned short*)malloc(gorder * sizeof(unsigned short));

            nodelist[i].visited = 1;
            EnqueueGraphNode(i, &queue);

            while (!IsGraphQueueEmpty(&queue)) {
                unsigned currentNode = DequeueGraphNode(&queue);
                for (unsigned j = 0; j < nodelist[currentNode].nedges; j++) {
                    unsigned neighbor = nodelist[currentNode].edges[j];
                    if (!nodelist[neighbor].visited) {
                        nodelist[neighbor].visited = 1;
                        EnqueueGraphNode(neighbor, &queue);
                    }
                }
            }

            free(queue.array);
        }
    }
    return components;
}

/* 
---------------------------
3. Command-line Argument Processing 
---------------------------
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

    int connectedComponents = CountConnectedComponents(nodelist, gorder);
    printf("Number of connected components: %d\n", connectedComponents);

    free(nodelist); // Clean up memory
}
