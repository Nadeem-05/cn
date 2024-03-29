#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <limits>

using namespace std;

class Graph {
    unordered_map<char, vector<pair<char, int>>> adjList;

public:
    void addEdge(char u, char v, int w) {
        adjList[u].push_back({v, w});
        adjList[v].push_back({u, w});
    }

    vector<char> shortestPath(char start, char end) {
        unordered_map<char, int> dist;
        unordered_map<char, char> parent;
        priority_queue<pair<int, char>, vector<pair<int, char>>, greater<pair<int, char>>> pq;

        for (auto& it : adjList)
            dist[it.first] = numeric_limits<int>::max();

        dist[start] = 0;
        pq.push({0, start});

        while (!pq.empty()) {
            char u = pq.top().second;
            pq.pop();

            for (auto& neighbor : adjList[u]) {
                char v = neighbor.first;
                int weight = neighbor.second;

                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    parent[v] = u;
                    pq.push({dist[v], v});
                }
            }
        }

        vector<char> path;
        for (char at = end; at != start; at = parent[at])
            path.push_back(at);
        path.push_back(start);
        reverse(path.begin(), path.end());

        return path;
    }
};

int main() {
    Graph network;
    network.addEdge('A', 'B', 2);
    network.addEdge('A', 'C', 4);
    network.addEdge('B', 'C', 1);
    network.addEdge('B', 'D', 4);
    network.addEdge('C', 'D', 2);

    char source = 'A';
    char destination = 'D';

    vector<char> shortestPath = network.shortestPath(source, destination);

    cout << "Shortest Path from " << source << " to " << destination << ": ";
    for (char node : shortestPath)
        cout << node << " ";
    cout << endl;

    return 0;
}
