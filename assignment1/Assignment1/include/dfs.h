#ifndef DFS_H
#define DFS_H

#include "csr_graph.h"
#include <vector>

struct DFSResult {
    std::vector<int> traversal;
};
DFSResult dfs(const CSRGraph &g, int source);

#endif
