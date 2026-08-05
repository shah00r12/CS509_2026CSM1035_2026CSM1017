#ifndef CSR_GRAPH_H
#define CSR_GRAPH_H

#include <vector>
#include <string>
#include <utility>

// One entry in an adjacency list: (neighbor_id, weight).
// weight is ignored for unweighted graphs (BFS/DFS use it).
using AdjEntry = std::pair<int, double>;
using AdjList  = std::vector<std::vector<AdjEntry>>;

// Compressed Sparse Row representation of a graph.
//   row_ptr[u] .. row_ptr[u+1]-1  gives the index range in col_idx/values for vertex u's neighbors.
//   col_idx[e]  = neighbor vertex id for edge e
//   values[e]   = edge weight for edge e (all 1.0 if unweighted, unused by BFS/DFS)
struct CSRGraph {
    int V = 0;
    bool weighted = false;
    std::vector<int> row_ptr;   // size V+1
    std::vector<int> col_idx;   // size = total directed-edge entries
    std::vector<double> values; // size = total directed-edge entries
};

// Reads an unweighted adjacency-list file in the assignment's format:
//   V E
//   u0 degree n1 n2 ...
//   ...
//   SOURCE s
// Fills adj (size V) and returns the source vertex.
int read_unweighted_adjlist(const std::string &path, int &V, int &E, AdjList &adj);

// Reads a positive-weighted adjacency-list file:
//   V E
//   u0 degree n1 w1 n2 w2 ...
//   ...
//   SOURCE s
int read_weighted_adjlist(const std::string &path, int &V, int &E, AdjList &adj);

// Converts an adjacency list into CSR format.
// This is PREPROCESSING per the assignment spec — its runtime must never
// be included in any reported algorithm execution time.
CSRGraph adjlist_to_csr(const AdjList &adj, bool weighted);

void print_csr(const CSRGraph &g);

#endif
