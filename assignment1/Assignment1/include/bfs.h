#ifndef BFS_H
#define BFS_H

#include "csr_graph.h"
#include <vector>

struct BFSResult {
    std::vector<int> traversal; // order vertices were visited in
    std::vector<int> distance;  // min edge-count distance from source; -1 if unreachable
};

// Runs BFS from 'source' over a CSR graph. Assumes g already built (untimed setup
// happened before this call). This function itself is what should be timed.
BFSResult bfs(const CSRGraph &g, int source);

#endif
