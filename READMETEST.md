# Assignment - Graph Properties Examiner

This repository contains code written for an assignment as part of the Modelling for Science and Engineering MSc at the Universitat Autònoma de Barcelona during the academic year 2023 - 2024. The assignment focuses on examining various properties of graphs in the context of graph theory.

## How to Run

1. Compilation: to compile a specific program (e.g., Ex0.c), use the following command:
   ```bash
   gcc Ex0.c -o Ex0

2. Execution: execute the program with a sample graph file (e.g., SampleGraph.txt) by running:
   ```bash
    ./Ex0 SampleGraph.txt





## Usage

To use this program, execute it with a `.txt` file containing a sample graph as a command-line argument. The graph's information should be formatted as exemplified in the file 'SampleGraph.txt'.

**Limitations:**

- This program evaluates one graph at a time.
- It only accepts graphs with a number of edges less than a specified integer (changeable).
- The program treats edges as undirected.
- The maximum number of edges is initially set to 8 for memory-saving purposes, which can be adjusted for larger graphs.

## Approach

The program utilizes the breadth-first-search (BFS) algorithm to examine connectedness, the number of connected components, and whether the graph is a tree. The BFS algorithm systematically traverses the graph in layers, exploring all neighbors of a node before moving on to their neighbors.

A queue structure is used to manage the order of node traversal in BFS. The queue is initialized with no elements. The BFS enqueues a source node and marks it as visited. It dequeues nodes corresponding to neighbors of the source node and continues this process, marking nodes as visited as the exploration progresses.

## Code Structure

In an effort to improve code readability, a `ReadGraphFromFile` function is introduced. This function handles memory allocation for the graph data structure (`nodelist`) and passes allocated memory back to the main function. This approach enhances code modularity and organization.

## Programs Included

### Ex0.c: Basic Graph Information

Ex0.c examines the information encoded in the .txt file and provides a detailed explanation of the graph's properties, including its node-by-node connections.

### Ex1.c: Graph Connectedness

This program evaluates whether the graph is connected or not connected. The output is 1 for a connected graph and 0 for a disconnected graph.

### Ex2.c: Number of Connected Components

This script calculates and prints the number of connected components in the graph.

### Ex3.c: Is the Graph a Tree?

A tree is an undirected graph that is both connected and acyclic. Ex3.c evaluates these conditions to determine whether the provided graph is a tree or not. It prints "Graph is a tree" if both conditions are met, otherwise, it prints "Graph is not a tree."

---

*Author: Sofia Llàcer Caro*
