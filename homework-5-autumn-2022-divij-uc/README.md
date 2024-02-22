# Homework 5

Write a C program that solves the all source shortest path problem by applying Dijkstra n = |V| times –– that is, once for each vertex in the input graph G, as described in the recorded lectures. An important detail not described in typical Dijkstra pseudocode is the implementation of the next-closest vertex search. To get full credit, and to ensure the algorithm runs in reasonable time for the large input file provided, the search must be done using a binary min-heap priority queue*. To speed up execution of the large graph, you must also parallelize your code using OpenMP. Rather than save all of the path data, output only the 10 longest paths in a file with format:

start vertex 1, end vertex 1, distance 1<br>
start vertex 2, end vertex 2, distance 2<br>
...<br>
...<br>
start vertex 10, end vertex 10, distance 10

In addition to devising and running your own correctness tests, carry out a performance analysis on the two graphs provided. No credit will be given for extremely inefficient implementations (several hours is the expected execution time in serial for large graph).

*An excellent reference for building a min-heap priority queue can be found here: https://bradfieldcs.com/algos/trees/priority-queues-with-binary-heaps/

## ANSWER

We build the graoh using an array of linked lists where each array entry is a node, and the linked list from that array entry is the list of edges available to that node. The first entry in the linked list is to the node itself with a distance of 0.
We then implement djiskstra's algorithm using a min heap to find the minimum value. We keep adding to the heap if we find a lower distance and jsut pop out the lowest when necessary. If the heap returns a node that ahs already been processed, we keep popping further nodes until we hit one that hasnt been processed. We then compare each node against the list of 10 highest non-infinity node distances for each thread, and add it to the list if its higher than any of them. Post each thread running, we write a custom reduction that gives us the 10 largest nodes from the 2 threads that combine. We then print these 10 largest distances. 
A point to note is that these 10 largest nodes essentially include all edges with distances 103, 102, and 101, and as we are combining threads as and when they finish, we get some different edges in the top 10 for the edges with distances 101, depending on whichever thread finishes first. They are still meaningful, and correct, but the order is unpredictable. We do get the same 103 and 102 distance edges everytime as they are less than 10 in number combined.

The performance increase due to using a min heap is impressive.

For graph 1, a linear search through the array gives us a time of 3603 seconds (~1 hour), while the heap gives us a timing of 12s. Once we parallelize on the linux cluster, we get it down to 1.2 seconds.

For graph 2, we get results anywhere from 33 to 36 minutes on the shared linux cluster using 16 threads on 8 cores. These could be made faster using a dedicated node.

Given the way we are initializing the array, we do have to make some checks regarding NULL values etc that seems to lead to a slower than optimal solution. We could make this faster by rewriting the graph structure to be faster and more robust.

An important consideration is making the schedule dynamic, as the number of edges per node is unequal, a static schedule leads to some threads finishing work faster than the others, and therefore just waiting for slow threads to finish. A dynamic schedule of a 100 nodes per allocation worked well.

### Time taken (Fastest run)
For graph2.txt on 8 cores (shared workload on linux cluster in seconds) -
TOTAL TIME: 1984.3144
TIME PER NODE: 0.0075

### 10 longest distances
#### Run 1 

100201 36397 103
100201 150246 103
41354 36397 102
41354 150246 102
100201 92762 102
100201 139117 102
46297 36397 101
46297 150246 101
40743 36397 101
40743 150246 101

#### Run 2
100201 36397 103
100201 150246 103
41354 150246 102
41354 36397 102
100201 92762 102
100201 139117 102
41354 92762 101
41354 139117 101
46297 36397 101
46297 150246 101
