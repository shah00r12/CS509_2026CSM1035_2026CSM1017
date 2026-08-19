#ifndef MAXFLOW_MINCUT_H
#define MAXFLOW_MINCUT_H

#include "csr_graph.h"
#include <vector>
#include <array>
#include <string>
struct MaxflowInput {
    int V = 0;
    int E = 0;
    AdjList adj;   
    int source = -1;
    int sink = -1;
};

struct MaxflowResult {
    long long max_flow = 0;
    long long min_cut_capacity = 0;
    std::vector<int> source_side;   
    std::vector<int> sink_side;    
    std::vector<std::array<long long, 3>> cut_edges; 
};

MaxflowInput read_maxflow_input(const std::string &path);
MaxflowResult compute_maxflow_mincut(const CSRGraph &g, int source, int sink);

#endif
