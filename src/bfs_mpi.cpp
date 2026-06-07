#include "graph.hpp"
#include <mpi.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>

std::vector<int> bfs_mpi(const Graph& graph, int source, int rank, int size) {
    std::vector<int> distance(graph.numVertices, -1);

    std::vector<int> frontier;

    if (rank == 0) {
        distance[source] = 0;
        frontier.push_back(source);
    }

    // Broadcast initial distance array
    MPI_Bcast(distance.data(), graph.numVertices, MPI_INT, 0, MPI_COMM_WORLD);

    int level = 0;

    while (true) {
        int local_frontier_size = frontier.size();
        int global_frontier_size = 0;

        MPI_Allreduce(
            &local_frontier_size,
            &global_frontier_size,
            1,
            MPI_INT,
            MPI_SUM,
            MPI_COMM_WORLD
        );

        if (global_frontier_size == 0) {
            break;
        }

        std::vector<int> local_next_frontier;

        for (int i = 0; i < (int)frontier.size(); i++) {
            int current = frontier[i];

            // Simple vertex ownership rule
            if (current % size != rank) {
                continue;
            }

            for (int neighbor : graph.adj[current]) {
                if (distance[neighbor] == -1) {
                    distance[neighbor] = level + 1;
                    local_next_frontier.push_back(neighbor);
                }
            }
        }

        // Gather local next frontier sizes
        int local_count = local_next_frontier.size();
        std::vector<int> recv_counts(size);

        MPI_Allgather(
            &local_count,
            1,
            MPI_INT,
            recv_counts.data(),
            1,
            MPI_INT,
            MPI_COMM_WORLD
        );

        std::vector<int> displs(size, 0);
        int total_count = 0;

        for (int i = 0; i < size; i++) {
            displs[i] = total_count;
            total_count += recv_counts[i];
        }

        std::vector<int> global_next_frontier(total_count);

        MPI_Allgatherv(
            local_next_frontier.data(),
            local_count,
            MPI_INT,
            global_next_frontier.data(),
            recv_counts.data(),
            displs.data(),
            MPI_INT,
            MPI_COMM_WORLD
        );

        // Update distance array consistently on all ranks
        for (int v : global_next_frontier) {
            if (distance[v] == -1) {
                distance[v] = level + 1;
            }
        }

        // Remove duplicates
        std::sort(global_next_frontier.begin(), global_next_frontier.end());
        global_next_frontier.erase(
            std::unique(global_next_frontier.begin(), global_next_frontier.end()),
            global_next_frontier.end()
        );

        frontier = global_next_frontier;
        level++;
    }

    return distance;
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::string filename = "data/test_graph_10.txt";
    if (argc > 1) {
	filename = argv[1];
    }
    Graph graph;
    graph.loadFromFile(filename);

    int source = 0;

    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();

    std::vector<int> distance = bfs_mpi(graph, source, rank, size);

    MPI_Barrier(MPI_COMM_WORLD);
    double end = MPI_Wtime();

    if (rank == 0) {
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

        std::cout << "\nExecution time: " << (end - start) << " seconds\n";
        std::cout << "MPI processes used: " << size << std::endl;
    }

    MPI_Finalize();

    return 0;
}
