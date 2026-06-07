#!/bin/bash

GRAPH=data/test_graph_10.txt

echo ""
echo "===================="
echo "Serial BFS"
echo "===================="
./bfs_serial $GRAPH

echo ""
echo "===================="
echo "OpenMP BFS"
echo "===================="
OMP_NUM_THREADS=4 ./bfs_openmp $GRAPH

echo ""
echo "===================="
echo "MPI BFS"
echo "===================="
mpirun -np 4 ./bfs_mpi $GRAPH

echo ""
echo "===================="
echo "Hybrid BFS"
echo "===================="
OMP_NUM_THREADS=2 mpirun -np 2 ./bfs_hybrid $GRAPH
