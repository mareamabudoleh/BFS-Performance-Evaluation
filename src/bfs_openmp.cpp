#include "graph.hpp"
#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>
#include <atomic>

std::vector<int> bfs_openmp(const Graph& graph, int source) {
    std::vector<std::atomic<int>> distance(graph.numVertices);

    for (int i = 0; i < graph.numVertices; i++) {
        distance[i].store(-1);
    }

    std::vector<int> frontier;
    frontier.push_back(source);
    distance[source].store(0);

    int level = 0;

    while (!frontier.empty()) {
        std::vector<int> next_frontier;

        #pragma omp parallel
        {
            std::vector<int> local_next_frontier;

            #pragma omp for schedule(dynamic)
            for (int i = 0; i < (int)frontier.size(); i++) {
                int current = frontier[i];

                for (int neighbor : graph.adj[current]) {
                    int expected = -1;

                    if (distance[neighbor].compare_exchange_strong(expected, level + 1)) {
                        local_next_frontier.push_back(neighbor);
                    }
                }
            }

            #pragma omp critical
            {
                next_frontier.insert(
                    next_frontier.end(),
                    local_next_frontier.begin(),
                    local_next_frontier.end()
                );
            }
        }

        frontier = next_frontier;
        level++;
    }

    std::vector<int> final_distance(graph.numVertices);

    for (int i = 0; i < graph.numVertices; i++) {
        final_distance[i] = distance[i].load();
    }

    return final_distance;
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

    std::vector<int> distance = bfs_openmp(graph, source);

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
    std::cout << "Threads used: " << omp_get_max_threads() << std::endl;

    return 0;
}
