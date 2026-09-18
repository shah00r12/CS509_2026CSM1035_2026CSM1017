#include "bfs.h"
#include <queue>

BFSResult bfs(const CSRGraph &g, int source) {
    BFSResult result;
    result.distance.assign(g.V, -1);
    result.traversal.reserve(g.V);

    std::queue<int> q;
    result.distance[source] = 0;
    q.push(source);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        result.traversal.push_back(u);
        for (int e = g.row_ptr[u]; e < g.row_ptr[u + 1]; ++e) {
            int v = g.col_idx[e];
            if (result.distance[v] == -1) {
                result.distance[v] = result.distance[u] + 1;
                q.push(v);
            }
        }
    }

    return result;
}
