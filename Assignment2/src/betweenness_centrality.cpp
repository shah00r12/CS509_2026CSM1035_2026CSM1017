#include "betweenness_centrality.h"
#include <queue>
#include <stack>

BetweennessCentralityResult compute_betweenness_centrality(const CSRGraph &g) {
    const int V = g.V;
    BetweennessCentralityResult res;
    res.centrality.assign(V, 0.0);

    std::vector<int> sigma(V);
    std::vector<int> dist(V);
    std::vector<double> delta(V);
    std::vector<std::vector<int>> preds(V);
    std::vector<int> order;
    order.reserve(V);

    for (int s = 0; s < V; ++s) {
        for (int v = 0; v < V; ++v) {
            preds[v].clear();
            sigma[v] = 0;
            dist[v] = -1;
            delta[v] = 0.0;
        }
        sigma[s] = 1;
        dist[s] = 0;

        order.clear();
        std::queue<int> q;
        q.push(s);

        while (!q.empty()) {
            int v = q.front();
            q.pop();
            order.push_back(v);

            int start = g.row_ptr[v];
            int end   = g.row_ptr[v + 1];
            for (int idx = start; idx < end; ++idx) {
                int w = g.col_idx[idx];
                if (dist[w] < 0) {
                    dist[w] = dist[v] + 1;
                    q.push(w);
                }
                if (dist[w] == dist[v] + 1) {
                    sigma[w] += sigma[v];
                    preds[w].push_back(v);
                }
            }
        }

        for (int i = static_cast<int>(order.size()) - 1; i >= 0; --i) {
            int w = order[i];
            for (int v : preds[w]) {
                delta[v] += (static_cast<double>(sigma[v]) / sigma[w]) * (1.0 + delta[w]);
            }
            if (w != s) {
                res.centrality[w] += delta[w];
            }
        }
    }
    for (int v = 0; v < V; ++v) {
        res.centrality[v] /= 2.0;
    }

    return res;
}
