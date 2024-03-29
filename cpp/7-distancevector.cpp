
#include <iostream>
#include <vector>
#include <limits>

// Structure to represent a link between routers
struct Link {
    int dest;
    int cost;
};

// Structure to represent a router
struct Router {
    int id;
    std::vector<Link> links;
};

// Function to initialize routing table for all routers
void initRoutingTable(std::vector<Router>& routers, int numRouters, int source) {
    for (int i = 0; i < numRouters; ++i) {
        if (i == source) {
            routers[i].id = i;
            routers[i].links.push_back({i, 0}); // Distance to itself is 0
        } else {
            routers[i].id = i;
            routers[i].links.push_back({i, std::numeric_limits<int>::max()}); // Initialize to infinity
        }
    }
}

// Function to perform distance vector routing
void distanceVectorRouting(std::vector<Router>& routers, int numRouters) {
    bool updated = true;
    while (updated) {
        updated = false;
        for (int i = 0; i < numRouters; ++i) {
            for (const auto& link : routers[i].links) {
                int dest = link.dest;
                int cost = link.cost;
                for (const auto& neighbor : routers[dest].links) {
                    int neighborDest = neighbor.dest;
                    int neighborCost = neighbor.cost;
                    if (routers[i].links[dest].cost > cost + neighborCost) {
                        routers[i].links[dest].cost = cost + neighborCost;
                        updated = true;
                    }
                }
            }
        }
    }
}

// Function to print routing table of all routers
void printRoutingTable(const std::vector<Router>& routers, int numRouters, int source) {
    std::cout << "Routing Table for Source Router " << source << ":\n";
    for (int i = 0; i < numRouters; ++i) {
        std::cout << "Destination: " << i << " | Cost: " << routers[source].links[i].cost << " | Next Hop: ";
        if (routers[source].links[i].cost == 0) {
            std::cout << "Self\n";
        } else if (routers[source].links[i].cost == std::numeric_limits<int>::max()) {
            std::cout << "Not Reachable\n";
        } else {
            int nextHop = i;
            while (routers[source].links[nextHop].cost != 0) {
                for (const auto& link : routers[nextHop].links) {
                    if (routers[source].links[nextHop].cost - link.cost == routers[source].links[i].cost) {
                        nextHop = link.dest;
                        break;
                    }
                }
            }
            std::cout << nextHop << "\n";
        }
    }
    std::cout << std::endl;
}

int main() {
    // Number of routers in the network
    const int numRouters = 5;

    // Create routers and initialize their routing tables
    std::vector<Router> routers(numRouters);
    for (int i = 0; i < numRouters; ++i) {
        initRoutingTable(routers, numRouters, i);
    }

    // Define links between routers (source, destination, cost)
    routers[0].links.push_back({1, 2});
    routers[0].links.push_back({2, 3});
    routers[1].links.push_back({0, 2});
    routers[1].links.push_back({2, 1});
    routers[1].links.push_back({3, 3});
    routers[2].links.push_back({0, 3});
    routers[2].links.push_back({1, 1});
    routers[2].links.push_back({3, 1});
    routers[2].links.push_back({4, 5});
    routers[3].links.push_back({1, 3});
    routers[3].links.push_back({2, 1});
    routers[3].links.push_back({4, 2});
    routers[4].links.push_back({2, 5});
    routers[4].links.push_back({3, 2});

    // Perform distance vector routing
    distanceVectorRouting(routers, numRouters);

    // Print routing tables for all routers
    for (int i = 0; i < numRouters; ++i) {
        printRoutingTable(routers, numRouters, i);
    }

    return 0;
}
