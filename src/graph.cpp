#include "graph.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

Graph::Graph() {
    numVertices = 0;
    numEdges = 0;
}

void Graph::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        exit(1);
    }

    std::string line;
    int src, dst;
    int maxVertex = -1;
    std::vector<std::pair<int, int>> edges;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }

        std::stringstream ss(line);
        if (ss >> src >> dst) {
            edges.push_back({src, dst});
            maxVertex = std::max(maxVertex, std::max(src, dst));
        }
    }

    file.close();

    numVertices = maxVertex + 1;
    numEdges = edges.size();
    adj.resize(numVertices);

    for (const auto& edge : edges) {
        adj[edge.first].push_back(edge.second);

        // Treat graph as undirected for BFS experiments.
        // Remove this line later for directed BFS.
        adj[edge.second].push_back(edge.first);
    }

    std::cout << "Graph loaded successfully." << std::endl;
    std::cout << "Vertices: " << numVertices << std::endl;
    std::cout << "Edges: " << numEdges << std::endl;
}

void Graph::printGraph() const {
    for (int i = 0; i < numVertices; i++) {
        std::cout << i << ": ";
        for (int neighbor : adj[i]) {
            std::cout << neighbor << " ";
        }
        std::cout << std::endl;
    }
}
