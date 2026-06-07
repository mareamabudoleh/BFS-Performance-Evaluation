#include <iostream>
#include <fstream>
#include <random>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: ./generate_rmat <scale> <edges> <output_file>\n";
        std::cerr << "Example: ./generate_rmat 12 50000 datasets/rmat_s12_e50k.txt\n";
        return 1;
    }

    int scale = std::stoi(argv[1]);
    int numEdges = std::stoi(argv[2]);
    std::string outputFile = argv[3];

    int numVertices = 1 << scale;

    double a = 0.57;
    double b = 0.19;
    double c = 0.19;
    double d = 0.05;

    std::mt19937 gen(42);
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    std::ofstream out(outputFile);

    if (!out.is_open()) {
        std::cerr << "Error: could not open output file.\n";
        return 1;
    }

    out << "# R-MAT synthetic graph\n";
    out << "# Nodes: " << numVertices << "\n";
    out << "# Edges: " << numEdges << "\n";

    for (int e = 0; e < numEdges; e++) {
        int src = 0;
        int dst = 0;
        int step = numVertices / 2;

        while (step > 0) {
            double r = dist(gen);

            if (r < a) {
                // top-left
            } else if (r < a + b) {
                dst += step;
            } else if (r < a + b + c) {
                src += step;
            } else {
                src += step;
                dst += step;
            }

            step /= 2;
        }

        if (src != dst) {
            out << src << " " << dst << "\n";
        }
    }

    out.close();

    std::cout << "R-MAT graph generated successfully.\n";
    std::cout << "Vertices: " << numVertices << "\n";
    std::cout << "Target edges: " << numEdges << "\n";
    std::cout << "Output: " << outputFile << "\n";

    return 0;
}
