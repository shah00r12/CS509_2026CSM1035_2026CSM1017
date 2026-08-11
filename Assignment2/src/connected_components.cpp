#include "connected_components.h"
#include <queue>

ConnectedComponentsResult find_connected_components(const CSRGraph &g) {
    const int V = g.V;
    ConnectedComponentsResult res;
    res.component.assign(V, -1);

    std::queue<int> q;

    for (int start = 0; start < V; ++start) {
        if (res.component[start] != -1) continue;

        int comp_id = res.num_components++;
        res.component[start] = comp_id;
        q.push(start);

        while (!q.empty()) {
            int v = q.front();
            q.pop();

            int begin = g.row_ptr[v];
            int end   = g.row_ptr[v + 1];
            for (int idx = begin; idx < end; ++idx) {
                int w = g.col_idx[idx];
                if (res.component[w] == -1) {
                    res.component[w] = comp_id;
                    q.push(w);
                }
            }
        }
    }

    return res;
}
