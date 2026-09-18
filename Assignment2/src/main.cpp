#include "csr_graph.h"
#include "triangle_counting.h"
#include "betweenness_centrality.h"
#include "connected_components.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <stdexcept>

using Clock = std::chrono::high_resolution_clock;

static constexpr int TRIANGLE_LIST_MAX_V = 100;

static void run_triangle_counting(const std::string &path) {
    int V, E;
    AdjList adj;

    read_unweighted_adjlist(path, V, E, adj);
    CSRGraph g = adjlist_to_csr(adj, false);

    bool collect_list = (V <= TRIANGLE_LIST_MAX_V);

    auto t1 = Clock::now();
    TriangleCountResult r = count_triangles(g, collect_list);
    auto t2 = Clock::now();
    double ms = std::chrono::duration<double, std::milli>(t2 - t1).count();

    std::cout << "Algorithm: Triangle Counting\n";
    std::cout << "Total triangles: " << r.total << "\n";
    if (collect_list) {
        std::cout << "Triangles found:\n";
        for (const auto &tri : r.triangles) {
            std::cout << "(" << tri[0] << ", " << tri[1] << ", " << tri[2] << ")\n";
        }
    }
    std::cout << "Execution time: " << ms << " ms\n";
}

static void run_betweenness_centrality(const std::string &path) {
    int V, E;
    AdjList adj;

    read_unweighted_adjlist(path, V, E, adj);
    CSRGraph g = adjlist_to_csr(adj,false);

    auto t1 = Clock::now();
    BetweennessCentralityResult r = compute_betweenness_centrality(g);
    auto t2 = Clock::now();
    double ms = std::chrono::duration<double, std::milli>(t2 - t1).count();

    std::cout << "Algorithm: Betweenness Centrality\n";
    std::cout << "Vertex Centrality\n";
    std::cout << std::fixed << std::setprecision(2);
    for (int v = 0; v < V; ++v) {
        std::cout << v << " " << r.centrality[v] << "\n";
    }
    std::cout << "Execution time: " << ms << " ms\n";
}

static void run_connected_components(const std::string &path) {
    int V, E;
    AdjList adj;

    read_unweighted_adjlist(path, V, E, adj);
    CSRGraph g = adjlist_to_csr(adj, false);

    auto t1 = Clock::now();
    ConnectedComponentsResult r = find_connected_components(g);
    auto t2 = Clock::now();
    double ms = std::chrono::duration<double, std::milli>(t2 - t1).count();

    std::cout << "Algorithm: Connected Components\n";
    std::cout << "Number of components: " << r.num_components << "\n";
    std::cout << "Vertex Component\n";
    for (int v = 0; v < V; ++v) {
        std::cout << v << " " << r.component[v] << "\n";
    }
    std::cout << "Execution time: " << ms << " ms\n";
}

static void print_usage(const char *prog) {
    std::cerr << "Usage:\n"
              << "  " << prog << " trianglecount  <input_file>\n"
              << "  " << prog << " betweenness    <input_file>\n"
              << "  " << prog << " connectedcomp  <input_file>\n";
}

int main(int argc, char **argv) {
    if (argc < 3) {
        print_usage(argv[0]);
        return 1;
    }

    std::string mode = argv[1];
    std::string path = argv[2];

    try {
        if (mode == "trianglecount")       run_triangle_counting(path);
        else if (mode == "betweenness")    run_betweenness_centrality(path);
        else if (mode == "connectedcomp")  run_connected_components(path);
        else { print_usage(argv[0]); return 1; }
    } catch (const std::exception &ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }
    
    return 0;
}
