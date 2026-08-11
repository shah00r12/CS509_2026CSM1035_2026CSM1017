#ifndef TRIANGLE_COUNTING_H
#define TRIANGLE_COUNTING_H

#include "csr_graph.h"
#include <vector>
#include <array>
#include <cstdint>

struct TriangleCountResult {
    long long total = 0;
    std::vector<std::array<int, 3>> triangles;
};

TriangleCountResult count_triangles(const CSRGraph &g, bool collect_list);

#endif
