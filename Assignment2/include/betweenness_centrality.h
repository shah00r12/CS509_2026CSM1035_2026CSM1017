#ifndef BETWEENNESS_CENTRALITY_H
#define BETWEENNESS_CENTRALITY_H

#include "csr_graph.h"
#include <vector>

struct BetweennessCentralityResult {
    std::vector<double> centrality;
};

BetweennessCentralityResult compute_betweenness_centrality(const CSRGraph &g);

#endif
