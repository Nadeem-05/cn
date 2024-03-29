class Router:
    def __init__(self, id):
        self.id = id
        self.routing_table = {}

    def add_link(self, dest, cost):
        self.routing_table[dest] = cost


def initialize_routing_table(routers, num_routers, source):
    for i in range(num_routers):
        if i == source:
            routers[i] = Router(i)
            routers[i].add_link(i, 0)  # Distance to itself is 0
        else:
            routers[i] = Router(i)
            routers[i].add_link(i, float('inf'))  # Initialize to infinity


def distance_vector_routing(routers, num_routers):
    updated = True
    while updated:
        updated = False
        for i in range(num_routers):
            for dest, cost in routers[i].routing_table.items():
                for neighbor_dest, neighbor_cost in routers[dest].routing_table.items():
                    if routers[i].routing_table.get(neighbor_dest, float('inf')) > cost + neighbor_cost:
                        routers[i].routing_table[neighbor_dest] = cost + neighbor_cost
                        updated = True


def print_routing_table(routers, num_routers, source):
    print(f"Routing Table for Source Router {source}:")
    for i in range(num_routers):
        print(f"Destination: {i} | Cost: {routers[source].routing_table[i]} | Next Hop: ", end="")
        if routers[source].routing_table[i] == 0:
            print("Self")
        elif routers[source].routing_table[i] == float('inf'):
            print("Not Reachable")
        else:
            next_hop = i
            while routers[source].routing_table[next_hop] != 0:
                for link_dest, link_cost in routers[next_hop].routing_table.items():
                    if routers[source].routing_table[next_hop] - link_cost == routers[source].routing_table[i]:
                        next_hop = link_dest
                        break
            print(next_hop)
    print()


def main():
    num_routers = 5
    routers = [None] * num_routers

    source = int(input("Enter the source router ID: "))
    initialize_routing_table(routers, num_routers, source)

    # Define links between routers (source, destination, cost)
    routers[0].add_link(1, 2)
    routers[0].add_link(2, 3)
    routers[1].add_link(0, 2)
    routers[1].add_link(2, 1)
    routers[1].add_link(3, 3)
    routers[2].add_link(0, 3)
    routers[2].add_link(1, 1)
    routers[2].add_link(3, 1)
    routers[2].add_link(4, 5)
    routers[3].add_link(1, 3)
    routers[3].add_link(2, 1)
    routers[3].add_link(4, 2)
    routers[4].add_link(2, 5)
    routers[4].add_link(3, 2)

    # Perform distance vector routing
    distance_vector_routing(routers, num_routers)

    # Print routing tables for all routers
    for i in range(num_routers):
        print_routing_table(routers, num_routers, i)


if __name__ == "__main__":
    main()
