#!/bin/bash

echo "Building Serial BFS..."
g++ -O3 -std=c++17 src/graph.cpp src/bfs_serial.cpp -o bfs_serial

echo "Building OpenMP BFS..."
g++ -O3 -fopenmp -std=c++17 \
src/graph.cpp src/bfs_openmp.cpp \
-o bfs_openmp

echo "Building MPI BFS..."
mpicxx -O3 -std=c++17 \
src/graph.cpp src/bfs_mpi.cpp \
-o bfs_mpi

echo "Building Hybrid BFS..."
mpicxx -O3 -fopenmp -std=c++17 \
src/graph.cpp src/bfs_hybrid.cpp \
-o bfs_hybrid

echo "Building R-MAT Generator..."
g++ -O3 -std=c++17 \
src/generate_rmat.cpp \
-o generate_rmat

echo "Build completed."
