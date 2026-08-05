#ifndef DFS_H
#define DFS_H

#include "csr_graph.h"
#include <vector>

struct DFSResult {
    std::vector<int> traversal; // order vertices were first visited in
};

// Iterative DFS (explicit stack, not recursion) so it's safe on very large
// graphs (V up to 100,000) without risking a call-stack overflow.
// Visits neighbors in the same order they appear in the input file.
DFSResult dfs(const CSRGraph &g, int source);

#endif
