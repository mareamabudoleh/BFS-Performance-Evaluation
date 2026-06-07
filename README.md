# BFS Performance Evaluation

## Overview

This project presents a performance evaluation of four Breadth-First Search (BFS) implementations:

- Serial BFS
- OpenMP BFS
- MPI BFS
- Hybrid MPI+OpenMP BFS

The goal of the project is to compare the execution time, speedup, and efficiency of different parallel programming models when processing large graph datasets.

The implementations were developed in **C++17** using **OpenMP** and **OpenMPI** and evaluated on both synthetic and real-world graph datasets.

---

## Requirements

### Software

- Ubuntu Linux
- GCC 11+ or GCC 13+
- OpenMP
- OpenMPI

### Tested Environment

- Ubuntu 22.04
- GCC 11.4.0
- OpenMPI 4.1.2
- Oracle Cloud VM.Standard.E5.Flex
- 2 OCPUs (4 logical CPUs)
- 12 GB RAM

---

## Compilation

### Serial BFS

```bash
g++ -O3 -std=c++17 src/graph.cpp src/bfs_serial.cpp -o bfs_serial
```

### OpenMP BFS

```bash
g++ -O3 -fopenmp -std=c++17 \
src/graph.cpp src/bfs_openmp.cpp \
-o bfs_openmp
```

### MPI BFS

```bash
mpicxx -O3 -std=c++17 \
src/graph.cpp src/bfs_mpi.cpp \
-o bfs_mpi
```

### Hybrid MPI+OpenMP BFS

```bash
mpicxx -O3 -fopenmp -std=c++17 \
src/graph.cpp src/bfs_hybrid.cpp \
-o bfs_hybrid
```

### R-MAT Generator

```bash
g++ -O3 -std=c++17 \
src/generate_rmat.cpp \
-o generate_rmat
```

---

## Running Experiments

### Serial

```bash
./bfs_serial data/test_graph_10.txt
```

### OpenMP

```bash
OMP_NUM_THREADS=4 \
./bfs_openmp data/test_graph_10.txt
```

### MPI

```bash
mpirun -np 4 \
./bfs_mpi data/test_graph_10.txt
```

### Hybrid MPI+OpenMP

```bash
OMP_NUM_THREADS=2 \
mpirun -np 2 \
./bfs_hybrid data/test_graph_10.txt
```

---

## Datasets

### Real-World Datasets

- Email-Eu-Core
- Wiki-Vote
- LiveJournal

### Synthetic Datasets

- R-MAT Scale 12
- R-MAT Scale 14
- R-MAT Scale 15
- R-MAT Scale 16
- R-MAT Scale 17
- R-MAT Scale 18
- R-MAT Scale 19
- R-MAT Scale 20

Large datasets are not stored in this repository due to size limitations. They can be downloaded from the SNAP dataset collection or generated using the included R-MAT generator.

---

## Key Results

### LiveJournal Dataset

| Implementation | Runtime (s) |
| -------------- | ----------: |
| Serial         |       3.216 |
| OpenMP         |       1.889 |
| MPI            |       5.673 |
| Hybrid         |       5.902 |

### R-MAT Scale 20

| Implementation | Runtime (s) |
| -------------- | ----------: |
| Serial         |       0.521 |
| OpenMP         |       0.406 |
| MPI            |       1.013 |
| Hybrid         |       1.471 |

### Main Findings

- OpenMP consistently achieved the best performance among the evaluated parallel implementations.
- OpenMP achieved approximately **1.70× speedup** on the LiveJournal dataset.
- MPI and Hybrid implementations were limited by communication overhead and graph replication.
- The expected crossover point where Hybrid MPI+OpenMP outperforms pure MPI was not observed.

---

## Future Work

Potential extensions include:

- Distributed graph partitioning
- Communication-avoiding BFS techniques
- NUMA-aware execution
- Multi-node cluster evaluation
- Optimized Hybrid MPI+OpenMP implementations

---

## Author

**Maream Abudoleh**

PhD Student, Computer Engineering
Istanbul Sabahattin Zaim University (IZU)

Course Project: Parallel and Distributed Programming
