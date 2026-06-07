#include "graph.hpp"
#include <iostream>
#include <vector>
#include <queue>
#include <chrono>

std::vector<int> bfs_serial(const Graph& graph, int source) {
    std::vector<int> distance(graph.numVertices, -1);
    std::queue<int> q;

    distance[source] = 0;
    q.push(source);

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        for (int neighbor : graph.adj[current]) {
            if (distance[neighbor] == -1) {
                distance[neighbor] = distance[current] + 1;
                q.push(neighbor);
            }
        }
    }

    return distance;
}

int main(int argc, char* argv[]) {
    Graph graph;
    std::string filename = "data/test_graph_10.txt";

    if (argc > 1) {
    	filename = argv[1];
    }

    graph.loadFromFile(filename);

    int source = 0;

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<int> distance = bfs_serial(graph, source);

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    int reachable = 0;
    int unreachable = 0;
    int maxDepth = 0;

    for (int d : distance) {
        if (d == -1) {
            unreachable++;
        } else {
            reachable++;
            maxDepth = std::max(maxDepth, d);
        }
    }

    std::cout << "\nBFS Summary\n";
    std::cout << "Reachable vertices: " << reachable << std::endl;
    std::cout << "Unreachable vertices: " << unreachable << std::endl;
    std::cout << "Maximum BFS depth: " << maxDepth << std::endl;

    std::cout << "\nExecution time: " << elapsed.count() << " seconds\n";

    return 0;
}
