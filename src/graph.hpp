#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <string>

class Graph {
public:
    int numVertices;
    int numEdges;
    std::vector<std::vector<int>> adj;

    Graph();

    void loadFromFile(const std::string& filename);
    void printGraph() const;
};

#endif

