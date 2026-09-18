#ifndef SSSP_H
#define SSSP_H

#include "csr_graph.h"
#include <vector>
#include <limits>

struct SSSPResult {
    std::vector<double> distance; 
    std::vector<int> predecessor; 
};

constexpr double SSSP_INF = std::numeric_limits<double>::infinity();
SSSPResult sssp(const CSRGraph &g, int source);

#endif
