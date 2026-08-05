#ifndef SSSP_H
#define SSSP_H

#include "csr_graph.h"
#include <vector>
#include <limits>

struct SSSPResult {
    std::vector<double> distance; // shortest distance from source; INF if unreachable
    std::vector<int> predecessor; // predecessor[v] = previous vertex on shortest path, -1 if none
};

constexpr double SSSP_INF = std::numeric_limits<double>::infinity();

// Dijkstra's algorithm over a CSR graph with positive edge weights (values[]).
// Uses a binary min-heap (std::priority_queue) -> O((V + E) log V).
SSSPResult sssp(const CSRGraph &g, int source);

#endif
