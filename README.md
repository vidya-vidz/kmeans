# kmeans
Instructions to run the program
gcc main.c -o main -lm
./main

k-means clustering is a method of vector quantization, originally from signal processing, that is popular for cluster analysis in data mining. k-means clustering aims to partition n observations into k clusters in which each observation belongs to the cluster with the nearest mean, serving as a prototype of the cluster. This results in a partitioning of the data space into Voronoi cells.

Kmeans algorithm is composed of following steps
1.Place K points into the space represented by the objects that are being clustered. These points represent initial group centroids.
2.Assign each object to the group that has the closest centroid.
3.When all objects have been assigned, recalculate the positions of the K centroids.
4.Repeat Steps 2 and 3 until the centroids no longer move. This produces a separation of the objects into groups from which the metric to be minimized can be calculated.
