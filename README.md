This repository contains the code written for one of the assignments of the course Optimisation as part of the Modelling for Science and Engineering MSc at the Universitat Autònoma de Barcelona (academic year 2023 - 2024). Note that the code in Ex0.c was kindly provided by the professor as a basis to build our own. The aim of the assignment was to examine different properties of a given graph (in the context of graph theory).

In order to use this programme, it needs to be executed using a .txt file containing a sample graph as a command-line argument. The graph's information should be formatted as is exemplified in the file 'SampleGraph.txt', which corresponds to the following diagram:\
![Alt text](SampleGraphDiagram.png?raw=true "Sample graph's diagram")\
Note that the first line of the .txt file includes the order of the graph (number of nodes) and the second line includes the number of edges in the graph. From then on, information on the connections between nodes is provided. A limitation of the programmes in this repository is that only one graph can be evaluated at a time and also that the programme can, for now, only take graphs where the number of edges is less than a given integer and considers the edges to be undirected. The maximum number of edges is set to 8 in the scripts that are provided in the repository for memory saving purposes, as the graphs analysed did not have more than this number. However, this can be changed to suit bigger graphs.

[comment]: <> (Add a little instruction on how to run it (command-line).)

# Approach

The breadth-first-search (BFS) algorithm was implemented in order to performing the tasks of examining connectedness, number of connected components and whether the graph is a tree. BFS is an algorithm that traverses a graph in layers, exploring all neighbors of a node before moving on to their neighbors. This code uses BFS to explore a given graph, starting from a particular node and systematically visiting its neighbors.

A queue structure was used to manage the order of node traversal in BFS. First, the queue is initialized with no elements. Then, the BFS enqueues a node considered the source and marks it as visited. It dequeues a node from the front of the queue, corresponding to a neighbor of the source node. They are marked as visited as the exploration of the nodes in the queue progresses. This process continues, ensuring that all neighbors at the current level are visited before moving to the next level.

Note that the code of Ex0.c included most of the information in the main function. Even though this is appropriate for the extension of that part, as I started coding from part 1 on I realized that the code soon started to get very long. In order to increase its readability, I decided to move the part which reads the .txt file into a function before the main part of the program. This way, all the functions could be defined in the preable and then called in the main part of the code, making it easier to read and explain.

To introduce the ```ReadGraphFromFile``` function, a redistribution of memory allocation was performed in the script. This was achieved by encapsulating memory allocation within the ```ReadGraphFromFile``` function, which is responsible for allocating memory for the graph data structure (```nodelist```). By using a pointer to a pointer (```node** nodelist```) and a pointer (```unsigned* gorder```) as function parameters, the allocated memory for nodelist is passed back to the main function. This approach enhances code modularity and organization by isolating memory allocation within a dedicated function while maintaining clean and readable code in the main function.

# Programs included
## Ex0.c: Basic graph information
Ex0.c checks the information encoded in the .txt file and provides a detailed explanation of the propoerties of the graph, included its node-by-node connections. An output like such is expected if the .txt file is properly formatted:\
```
SampleGraph.txt defines a graph with 4 nodes and 4 edges.

Node 0 has 1 edges:
  0 --> 1
Node 1 has 3 edges:
  1 --> 0
  1 --> 2
  1 --> 3
Node 2 has 2 edges:
  2 --> 1
  2 --> 3
Node 3 has 2 edges:
  3 --> 1
  3 --> 2
```

## Ex1.c: Graph connectedness
This program examines the information of the .txt file and determines whether the graph is connected or not connected. This is done by evaluating whether all the nodes of the graph were visited in the BFS algorithm. The output of the program corresponds to 1 if the graph is connected and 0 if it is not connected.

## Ex2.c: Number of connected components
This script introduces a modification with respect to Ex1.c on part 2.3. the code (Connectedness). Here, the number of connected components is calculated and printed.

## Ex3.c: Is graph a tree?
A tree is an undirected graph which is connected and acyclic. This program incoporates these conditions in order to evaluate whether the graph ptovided as an argument is a tree or not. If these conditions are both true for a given graph, the program prints "Graph is a tree.". Otherways, it prints "Graph is not a tree.".

