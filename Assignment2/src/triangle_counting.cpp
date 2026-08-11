#include "triangle_counting.h"
#include <algorithm>

TriangleCountResult count_triangles(const CSRGraph &g, bool collect_list) {
    TriangleCountResult res;
    const int V = g.V;

    std::vector<std::vector<int>> nbrs(V);
    for (int u = 0; u < V; u++) {
        int start = g.row_ptr[u];
        int end   = g.row_ptr[u + 1];
        nbrs[u].assign(g.col_idx.begin() + start, g.col_idx.begin() + end);
        std::sort(nbrs[u].begin(), nbrs[u].end());
    }

    long long raw_count = 0;

    for (int u = 0; u < V; u++) {
        const std::vector<int> &nu = nbrs[u];
        for (std::size_t i = 0; i < nu.size(); i++) {
            for (std::size_t j = i + 1; j < nu.size(); j++) {
                int v = nu[i];
                int w = nu[j];
                if (std::binary_search(nbrs[v].begin(), nbrs[v].end(), w)) {
                    ++raw_count;
                    if (collect_list) {
                        std::array<int, 3> tri = {u, v, w};
                        std::sort(tri.begin(), tri.end());
                        res.triangles.push_back(tri);
                    }
                }
            }
        }
    }

    res.total = raw_count / 3;

    if (collect_list) {
        std::sort(res.triangles.begin(), res.triangles.end());
        res.triangles.erase(std::unique(res.triangles.begin(), res.triangles.end()),res.triangles.end());
    }

    return res;
}
