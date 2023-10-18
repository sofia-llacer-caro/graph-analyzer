// Author: Sofia Llàcer Caro
// NIA: 1692021

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>



// Define a structure for a node in the queue
typedef struct GraphNodeStructure {
    unsigned short node;
    struct GraphNodeStructure* next; // Linked structure
    unsigned numNeighbors;
    unsigned neighbors[8];
    int visited;
} GraphNodeStructure;

// Define the queue structure
typedef struct Queue {
    GraphNodeStructure* front;
    GraphNodeStructure* rear;
} Queue;

/*Define functions*/


    /*Queue*/
int IsGraphQueueEmpty(Queue* Q) {
    return (Q->front == NULL);
}

int EnqueueGraphNode(unsigned short nodeValue, Queue* Q) {
    GraphNodeStructure* newNode = (GraphNodeStructure*)malloc(sizeof(GraphNodeStructure));
    if (newNode == NULL)
        return 0;

    newNode->node = nodeValue;
    newNode->next = NULL;

    if (Q->front)
        Q->rear->next = newNode;
    else
        Q->front = newNode;
    Q->rear = newNode;

    return 1;
}

unsigned int DequeueGraphNode(Queue* Q) {
    if (IsGraphQueueEmpty(Q))
        return UINT_MAX;

    GraphNodeStructure* initialNode = Q->front;
    unsigned int nodeValue = initialNode->node;
    Q->front = Q->front->next;
    free(initialNode);

    return nodeValue;
}

    /*Read file*/

int ReadGraphFromFile(const char* fileName, GraphNodeStructure** nodeList, unsigned* numNodes, unsigned* numEdges) {
    FILE* graphFile = fopen(fileName, "r");
    if (graphFile == NULL) {
        printf("\nERROR: Data file not found.\n");
        return -1;
    }

    fscanf(graphFile, "%u", numNodes);
    fscanf(graphFile, "%u", numEdges);

    *nodeList = (GraphNodeStructure*)malloc(*numNodes * sizeof(GraphNodeStructure));

    for (unsigned i = 0; i < *numNodes; i++) {
        (*nodeList)[i].numNeighbors = 0;
        (*nodeList)[i].visited = 0;
    }

    for (unsigned j = 0; j < *numEdges; j++) {
        unsigned source, destination;
        fscanf(graphFile, "%u %u", &source, &destination);
        (*nodeList)[source].neighbors[(*nodeList)[source].numNeighbors] = destination;
        (*nodeList)[source].numNeighbors++;
        (*nodeList)[destination].neighbors[(*nodeList)[destination].numNeighbors] = source;
        (*nodeList)[destination].numNeighbors++;
    }

    fclose(graphFile);
    return 0;
}

    /*Connectedness*/

int IsGraphConnected(GraphNodeStructure* nodeList, unsigned numNodes) {
    Queue queue;
    queue.front = queue.rear = NULL;

    nodeList[0].visited = 1;
    EnqueueGraphNode(0, &queue);

    while (!IsGraphQueueEmpty(&queue)) {
        unsigned currentNode = DequeueGraphNode(&queue);
        for (unsigned i = 0; i < nodeList[currentNode].numNeighbors; i++) {
            unsigned neighbor = nodeList[currentNode].neighbors[i];
            if (!nodeList[neighbor].visited) {
                nodeList[neighbor].visited = 1;
                EnqueueGraphNode(neighbor, &queue);
            }
        }
    }

    for (unsigned i = 0; i < numNodes; i++) {
        if (nodeList[i].visited == 0) {
            return 0;
        }
    }

    return 1;
}










int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <graph_file>\n", argv[0]);
        return -1;
    }

    GraphNodeStructure* nodeList;
    unsigned numNodes, numEdges;

    if (ReadGraphFromFile(argv[1], &nodeList, &numNodes, &numEdges) != 0) {
        return -1;
    }

    printf("%s defines a graph with %u nodes and %u edges.\n", argv[1], numNodes, numEdges);

    int isConnected = IsGraphConnected(nodeList, numNodes);
    if (isConnected) {
        printf("The graph is connected.\n");
    } else {
        printf("The graph is not connected.\n");
    }

    free(nodeList);
    return isConnected;
}
