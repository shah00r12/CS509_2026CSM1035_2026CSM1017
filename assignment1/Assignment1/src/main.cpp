#include "csr_graph.h"
#include "bfs.h"
#include "dfs.h"
#include "sssp.h"
#include <iostream>
#include <chrono>
#include <stdexcept>

using Clock = std::chrono::high_resolution_clock;

static void print_distance_line(int v, int d) {
    if (d == -1) std::cout << v << " INF\n";
    else         std::cout << v << " " << d << "\n";
}

static void run_bfs(const std::string &path) {
    int V, E;
    AdjList adj;
    int source = read_unweighted_adjlist(path, V, E, adj);
    CSRGraph g = adjlist_to_csr(adj,false);
    auto t1 = Clock::now();
    BFSResult r = bfs(g, source);
    auto t2 = Clock::now();
    double ms = std::chrono::duration<double, std::milli>(t2 - t1).count();
    std::cout << "Algorithm: BFS\n";
    std::cout << "Source: " << source << "\n";
    std::cout << "Traversal: ";
    for (size_t i = 0; i < r.traversal.size(); ++i)
        std::cout << r.traversal[i] << (i + 1 < r.traversal.size() ? " " : "\n");
    if (r.traversal.empty()) std::cout << "\n";
    std::cout << "Distances:\n";
    for (int v = 0; v < V; ++v) print_distance_line(v, r.distance[v]);
    std::cout << "Execution time: " << ms << " ms\n";
}

static void run_dfs(const std::string &path) {
    int V, E;
    AdjList adj;

    int source = read_unweighted_adjlist(path, V, E, adj);
    CSRGraph g = adjlist_to_csr(adj, false);

    auto t1 = Clock::now();
    DFSResult r = dfs(g, source);
    auto t2 = Clock::now();
    double ms = std::chrono::duration<double, std::milli>(t2 - t1).count();

    std::cout << "Algorithm: DFS\n";
    std::cout << "Source: " << source << "\n";
    std::cout << "Traversal: ";
    for (size_t i = 0; i < r.traversal.size(); ++i)
        std::cout << r.traversal[i] << (i + 1 < r.traversal.size() ? " " : "\n");
    if (r.traversal.empty()) std::cout << "\n";
    std::cout << "Execution time: " << ms << " ms\n";
}

static void run_sssp(const std::string &path) {
    int V, E;
    AdjList adj;

    int source = read_weighted_adjlist(path, V, E, adj);
    CSRGraph g = adjlist_to_csr(adj, true);

    auto t1 = Clock::now();
    SSSPResult r = sssp(g, source);
    auto t2 = Clock::now();
    double ms = std::chrono::duration<double, std::milli>(t2 - t1).count();

    std::cout << "Algorithm: SSSP\n";
    std::cout << "Source: " << source << "\n";
    std::cout << "Vertex Distance\n";
    for (int v = 0; v < V; ++v) {
        std::cout << v << " ";
        if (r.distance[v] == SSSP_INF) std::cout << "INF\n";
        else std::cout << r.distance[v] << "\n";
    }
    std::cout << "Execution time: " << ms << " ms\n";
}

static void print_usage(const char *prog) {
    std::cerr << "Usage:\n"
              << "  " << prog << " bfs  <input_file>\n"
              << "  " << prog << " dfs  <input_file>\n"
              << "  " << prog << " sssp <input_file>\n";
}

int main(int argc, char **argv) {
    if (argc < 3) {
        print_usage(argv[0]);
        return 1;
    }

    std::string mode = argv[1];
    std::string path = argv[2];

    try {
        if (mode == "bfs")       run_bfs(path);
        else if (mode == "dfs")  run_dfs(path);
        else if (mode == "sssp") run_sssp(path);
        else { print_usage(argv[0]); return 1; }
    } catch (const std::exception &ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
