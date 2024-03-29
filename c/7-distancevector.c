#include <stdio.h>
#include <limits.h>

#define NUM_ROUTERS 5

// Structure to represent a router
typedef struct {
    int id;
    int routing_table[NUM_ROUTERS][2]; // [destination][cost, next_hop]
} Router;

// Function to initialize routing table for all routers
void init_routing_table(Router routers[], int source) {
    for (int i = 0; i < NUM_ROUTERS; ++i) {
        routers[i].id = i;
        for (int j = 0; j < NUM_ROUTERS; ++j) {
            if (i == source) {
                routers[i].routing_table[j][0] = (i == j) ? 0 : INT_MAX; // Distance to itself is 0, others are infinity
                routers[i].routing_table[j][1] = (i == j) ? i : -1; // Next hop is itself for self, -1 for others
            } else {
                routers[i].routing_table[j][0] = INT_MAX; // Initialize to infinity
                routers[i].routing_table[j][1] = -1; // Initialize next hop to -1
            }
        }
    }
}

// Function to perform distance vector routing
void distance_vector_routing(Router routers[]) {
    int updated;
    do {
        updated = 0;
        for (int i = 0; i < NUM_ROUTERS; ++i) {
            for (int j = 0; j < NUM_ROUTERS; ++j) {
                for (int k = 0; k < NUM_ROUTERS; ++k) {
                    if (routers[i].routing_table[j][0] > routers[i].routing_table[k][0] + routers[k].routing_table[j][0]) {
                        routers[i].routing_table[j][0] = routers[i].routing_table[k][0] + routers[k].routing_table[j][0];
                        routers[i].routing_table[j][1] = k;
                        updated = 1;
                    }
                }
            }
        }
    } while (updated);
}

// Function to print routing table of all routers
void print_routing_table(Router routers[], int source) {
    printf("Routing Table for Source Router %d:\n", source);
    for (int i = 0; i < NUM_ROUTERS; ++i) {
        printf("Destination: %d | Cost: %d | Next Hop: ", i, routers[source].routing_table[i][0]);
        if (routers[source].routing_table[i][0] == 0) {
            printf("Self\n");
        } else if (routers[source].routing_table[i][0] == INT_MAX) {
            printf("Not Reachable\n");
        } else {
            int next_hop = i;
            while (routers[source].routing_table[next_hop][0] != 0) {
                next_hop = routers[source].routing_table[next_hop][1];
            }
            printf("%d\n", next_hop);
        }
    }
    printf("\n");
}

int main() {
    Router routers[NUM_ROUTERS];
    int source;

    printf("Enter the source router ID: ");
    scanf("%d", &source);

    // Initialize routing tables for all routers
    init_routing_table(routers, source);

    // Define links between routers (source, destination, cost)
    routers[0].routing_table[1][0] = 2;
    routers[0].routing_table[2][0] = 3;
    routers[1].routing_table[0][0] = 2;
    routers[1].routing_table[2][0] = 1;
    routers[1].routing_table[3][0] = 3;
    routers[2].routing_table[0][0] = 3;
    routers[2].routing_table[1][0] = 1;
    routers[2].routing_table[3][0] = 1;
    routers[2].routing_table[4][0] = 5;
    routers[3].routing_table[1][0] = 3;
    routers[3].routing_table[2][0] = 1;
    routers[3].routing_table[4][0] = 2;
    routers[4].routing_table[2][0] = 5;
    routers[4].routing_table[3][0] = 2;

    // Perform distance vector routing
    distance_vector_routing(routers);

    // Print routing tables for all routers
    for (int i = 0; i < NUM_ROUTERS; ++i) {
        print_routing_table(routers, i);
    }

    return 0;
}
