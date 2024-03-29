import heapq

class Graph:
    def __init__(self):
        self.nodes = set()
        self.edges = {}
        self.distances = {}

    def add_node(self, value):
        self.nodes.add(value)

    def add_edge(self, from_node, to_node, distance):
        self.edges.setdefault(from_node, []).append(to_node)
        self.edges.setdefault(to_node, []).append(from_node)
        self.distances[(from_node, to_node)] = distance
        self.distances[(to_node, from_node)] = distance

    def dijkstra(self, initial):
        visited = {initial: 0}
        path = {}

        nodes = set(self.nodes)

        while nodes:
            min_node = None
            for node in nodes:
                if node in visited:
                    if min_node is None:
                        min_node = node
                    elif visited[node] < visited[min_node]:
                        min_node = node

            if min_node is None:
                break

            nodes.remove(min_node)
            current_weight = visited[min_node]

            for edge in self.edges[min_node]:
                weight = current_weight + self.distances[(min_node, edge)]
                if edge not in visited or weight < visited[edge]:
                    visited[edge] = weight
                    path[edge] = min_node

        return visited, path

    def shortest_path(self, start, end):
        distances, paths = self.dijkstra(start)
        route = []
        while end != start:
            route.append(end)
            end = paths[end]
        route.append(start)
        route.reverse()
        return route

# Example Usage:
if __name__ == "__main__":
    network = Graph()
    network.add_node("A")
    network.add_node("B")
    network.add_node("C")
    network.add_node("D")

    network.add_edge("A", "B", 2)
    network.add_edge("A", "C", 4)
    network.add_edge("B", "C", 1)
    network.add_edge("B", "D", 4)
    network.add_edge("C", "D", 2)

    source = "A"
    destination = "D"

    shortest_path = network.shortest_path(source, destination)
    print("Shortest Path from {} to {}: {}".format(source, destination, shortest_path))
