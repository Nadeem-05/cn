#include <stdio.h>
#include <limits.h>

#define MAX_NODES 10

// Structure to represent an edge
struct Edge {
    int destination;
    int weight;
};

// Structure to represent a graph
struct Graph {
    int numNodes;
    int adjMatrix[MAX_NODES][MAX_NODES];
};

// Function to find the vertex with minimum distance value
int minDistance(int dist[], int visited[], int numNodes) {
    int min = INT_MAX, min_index;
    for (int v = 0; v < numNodes; v++) {
        if (visited[v] == 0 && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

// Function to print the shortest path from source to destination
void printPath(int parent[], int j) {
    if (parent[j] == -1)
        return;
    printPath(parent, parent[j]);
    printf("%d ", j);
}

// Function to find and print shortest path using Dijkstra's algorithm
void dijkstra(struct Graph* graph, int source, int destination) {
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    int parent[MAX_NODES];

    // Initialization
    for (int i = 0; i < graph->numNodes; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
        parent[i] = -1;
    }

    dist[source] = 0;

    // Find shortest path for all nodes
    for (int count = 0; count < graph->numNodes - 1; count++) {
        int u = minDistance(dist, visited, graph->numNodes);
        visited[u] = 1;

        for (int v = 0; v < graph->numNodes; v++) {
            if (!visited[v] && graph->adjMatrix[u][v] && dist[u] != INT_MAX &&
                dist[u] + graph->adjMatrix[u][v] < dist[v]) {
                dist[v] = dist[u] + graph->adjMatrix[u][v];
                parent[v] = u;
            }
        }
    }

    // Print the shortest path from source to destination
    printf("Shortest Path from %d to %d: ", source, destination);
    printPath(parent, destination);
    printf("%d\n", destination);
}

int main() {
    struct Graph graph = {
        .numNodes = 4,
        .adjMatrix = {
            {0, 2, 4, 0},
            {2, 0, 1, 4},
            {4, 1, 0, 2},
            {0, 4, 2, 0}
        }
    };

    int source = 0;
    int destination = 3;

    dijkstra(&graph, source, destination);

    return 0;
}
