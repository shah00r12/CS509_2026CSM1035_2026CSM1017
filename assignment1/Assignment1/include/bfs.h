#ifndef BFS_H
#define BFS_H

#include "csr_graph.h"
#include <vector>

struct BFSResult {
    std::vector<int> traversal; 
    std::vector<int> distance; 
};
BFSResult bfs(const CSRGraph &g, int source);

#endif
