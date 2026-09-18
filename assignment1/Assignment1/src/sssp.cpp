#include "sssp.h"
#include <queue>
#include <vector>

SSSPResult sssp(const CSRGraph &g, int source) {
    SSSPResult result;
    result.distance.assign(g.V, SSSP_INF);
    result.predecessor.assign(g.V, -1);
    using PQEntry = std::pair<double, int>;
    std::priority_queue<PQEntry, std::vector<PQEntry>, std::greater<PQEntry>> pq;

    result.distance[source] = 0.0;
    pq.push({0.0, source});

    std::vector<char> finalized(g.V, 0);

    while (!pq.empty()) {
        PQEntry p = pq.top();
        double d = p.first;
        int u = p.second;
        pq.pop();

        if (finalized[u]) continue;
        finalized[u] = 1;

        for (int e = g.row_ptr[u]; e < g.row_ptr[u + 1]; ++e) {
            int v = g.col_idx[e];
            double w = g.values[e];
            double new_dist = d + w;
            if (new_dist < result.distance[v]) {
                result.distance[v] = new_dist;
                result.predecessor[v] = u;
                pq.push({new_dist, v});
            }
        }
    }

    return result;
}
