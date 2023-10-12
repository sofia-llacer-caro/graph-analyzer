This repository contains the code written for one of the assignments of the course Optimisation as part of the Modelling for Science and Engineering MSc at the Universitat Autònoma de Barcelona (academic year 2023 - 2024). Note that the code in Ex0.c was kindly provided by the professor as a basis to build our own. The aim of the assignment was to examine different properties of a given graph (in the context of graph theory).

In order to use this programme, it needs to be executed using a .txt file containing a sample graph as a command-line argument. The graph's information should be formatted as is exemplified in the file 'SampleGraph.txt', which corresponds to the following diagram:/
![Alt text](SampleGraphDiagram.png?raw=true "Sample graph's diagram")\
Note that the first line of the .txt file includes the order of the graph (number of nodes) and the second line includes the number of edges in the graph. From then on, information on the connections between nodes is provided.\

# 0. Basic graph information
Ex0.c 

# 1. Graph connectedness
Ex1.c : Checking the connectedness of a graph. Given the following definition of connectedness:
An undirected graph is connected when there is a path between each pair of vertices (i.e., there are no inaccessible vertices).
Where a connected component is taken to be the following:
A connected component of an undirected graph is a maximal connected subgraph. Note that each vertex and each edge belongs to a single connected component.