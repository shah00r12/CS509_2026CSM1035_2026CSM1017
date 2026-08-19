#include "csr_graph.h"
#include "gradient_descent.h"
#include "maxflow_mincut.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <stdexcept>

using Clock = std::chrono::high_resolution_clock;

static void run_gradient_descent_mode(const std::string &path) {
    GDInput in = read_gd_input(path);

    auto t1 = Clock::now();
    GDResult r = run_gradient_descent(in);
    auto t2 = Clock::now();
    double ms = std::chrono::duration<double, std::milli>(t2 - t1).count();

    std::cout << "Algorithm: Gradient Descent\n";
    std::cout << "Degree: " << r.degree << "\n";
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Final x: " << r.final_x << "\n";
    std::cout << "Final f(x): " << r.final_fx << "\n";
    std::cout << "Iterations: " << r.iterations << "\n";
    std::cout << "Converged: " << (r.converged ? "true" : "false") << "\n";
    std::cout << "Execution time: " << ms << " ms\n";
}

static void run_maxflow_mode(const std::string &path) {
    MaxflowInput in = read_maxflow_input(path);
    CSRGraph g = adjlist_to_csr(in.adj, /*weighted=*/true);

    auto t1 = Clock::now();
    MaxflowResult r = compute_maxflow_mincut(g, in.source, in.sink);
    auto t2 = Clock::now();
    double ms = std::chrono::duration<double, std::milli>(t2 - t1).count();

    std::cout << "Algorithm: Maxflow-Mincut\n";
    std::cout << "Source: " << in.source << "\n";
    std::cout << "Sink: " << in.sink << "\n";
    std::cout << "Maximum flow: " << r.max_flow << "\n";
    std::cout << "Minimum cut capacity: " << r.min_cut_capacity << "\n";

    std::cout << "Source side:";
    for (int v : r.source_side) std::cout << " " << v;
    std::cout << "\n";

    std::cout << "Sink side:";
    for (int v : r.sink_side) std::cout << " " << v;
    std::cout << "\n";

    std::cout << "Cut edges:\n";
    for (const auto &e : r.cut_edges) {
        std::cout << e[0] << " " << e[1] << " " << e[2] << "\n";
    }
    std::cout << "Execution time: " << ms << " ms\n";
}

static void print_usage(const char *prog) {
    std::cerr << "Usage:\n"
              << "  " << prog << " gradientdescent <input_file>\n"
              << "  " << prog << " maxflow         <input_file>\n";
}

int main(int argc, char **argv) {
    if (argc < 3) {
        print_usage(argv[0]);
        return 1;
    }

    std::string mode = argv[1];
    std::string path = argv[2];

    try {
        if (mode == "gradientdescent")   run_gradient_descent_mode(path);
        else if (mode == "maxflow")      run_maxflow_mode(path);
        else { print_usage(argv[0]); return 1; }
    } catch (const std::exception &ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
