#include "maxflow_mincut.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <queue>
#include <functional>
#include <climits>
#include <cmath>
#include <algorithm>

MaxflowInput read_maxflow_input(const std::string &path) {
    std::ifstream fin(path);
    if (!fin.is_open())
        throw std::runtime_error("Could not open input file: " + path);

    MaxflowInput in;
    if (!(fin >> in.V >> in.E))
        throw std::runtime_error("Malformed header line (expected: V E)");
    if (in.V <= 0)
        throw std::runtime_error("Invalid Maxflow-Mincut input: V must be positive");

    in.adj.assign(in.V, {});

    for (int line = 0; line < in.V; ++line) {
        int u, degree;
        if (!(fin >> u >> degree))
            throw std::runtime_error("Malformed adjacency line for vertex " + std::to_string(line));
        if (u < 0 || u >= in.V)
            throw std::runtime_error("Vertex id out of range: " + std::to_string(u));

        in.adj[u].reserve(degree);
        for (int d = 0; d < degree; ++d) {
            int nbr;
            double cap;
            if (!(fin >> nbr))
                throw std::runtime_error("Malformed neighbor list for vertex " + std::to_string(u));
            if (nbr < 0 || nbr >= in.V)
                throw std::runtime_error("Invalid Maxflow-Mincut input: neighbor vertex out of range: " + std::to_string(nbr));
            if (!(fin >> cap))
                throw std::runtime_error("Missing edge capacity for vertex " + std::to_string(u));
            if (cap < 0)
                throw std::runtime_error("Invalid Maxflow-Mincut input: edge capacities must be non-negative");
            in.adj[u].push_back({nbr, cap});
        }
    }

    std::string keyword;
    if (!(fin >> keyword) || keyword != "SOURCE")
        throw std::runtime_error("Malformed Maxflow-Mincut input: expected SOURCE line");
    if (!(fin >> in.source))
        throw std::runtime_error("Malformed Maxflow-Mincut input: missing source vertex");

    if (!(fin >> keyword) || keyword != "SINK")
        throw std::runtime_error("Malformed Maxflow-Mincut input: expected SINK line");
    if (!(fin >> in.sink))
        throw std::runtime_error("Malformed Maxflow-Mincut input: missing sink vertex");

    if (in.source < 0 || in.source >= in.V)
        throw std::runtime_error("Invalid Maxflow-Mincut input: source vertex out of range");
    if (in.sink < 0 || in.sink >= in.V)
        throw std::runtime_error("Invalid Maxflow-Mincut input: sink vertex out of range");
    if (in.source == in.sink)
        throw std::runtime_error("Invalid Maxflow-Mincut input: source and sink must be different vertices");

    return in;
}

MaxflowResult compute_maxflow_mincut(const CSRGraph &g, int source, int sink) {
    const int n = g.V;

    std::vector<int> to;
    std::vector<long long> cap;
    std::vector<std::vector<int>> adjEdges(n);

    auto add_edge = [&](int u, int v, long long c) {
        adjEdges[u].push_back(static_cast<int>(to.size()));
        to.push_back(v);
        cap.push_back(c);
        adjEdges[v].push_back(static_cast<int>(to.size()));
        to.push_back(u);
        cap.push_back(0);
    };

    for (int u = 0; u < n; ++u) {
        for (int idx = g.row_ptr[u]; idx < g.row_ptr[u + 1]; ++idx) {
            int v = g.col_idx[idx];
            long long c = static_cast<long long>(std::llround(g.values[idx]));
            add_edge(u, v, c);
        }
    }

    std::vector<int> level(n), iter_ptr(n);

    auto bfs = [&]() -> bool {
        std::fill(level.begin(), level.end(), -1);
        std::queue<int> q;
        level[source] = 0;
        q.push(source);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int eid : adjEdges[u]) {
                if (cap[eid] > 0 && level[to[eid]] < 0) {
                    level[to[eid]] = level[u] + 1;
                    q.push(to[eid]);
                }
            }
        }
        return level[sink] >= 0;
    };

    std::function<long long(int, long long)> dfs = [&](int u, long long f) -> long long {
        if (u == sink) return f;
        for (int &i = iter_ptr[u]; i < static_cast<int>(adjEdges[u].size()); ++i) {
            int eid = adjEdges[u][i];
            int v = to[eid];
            if (cap[eid] > 0 && level[v] == level[u] + 1) {
                long long d = dfs(v, std::min(f, cap[eid]));
                if (d > 0) {
                    cap[eid] -= d;
                    cap[eid ^ 1] += d;
                    return d;
                }
            }
        }
        return 0;
    };

    long long max_flow = 0;
    while (bfs()) {
        std::fill(iter_ptr.begin(), iter_ptr.end(), 0);
        long long f;
        while ((f = dfs(source, LLONG_MAX)) > 0) {
            max_flow += f;
        }
    }
    std::vector<bool> visited(n, false);
    std::queue<int> q;
    visited[source] = true;
    q.push(source);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int eid : adjEdges[u]) {
            if (cap[eid] > 0 && !visited[to[eid]]) {
                visited[to[eid]] = true;
                q.push(to[eid]);
            }
        }
    }

    MaxflowResult res;
    res.max_flow = max_flow;

    for (int v = 0; v < n; ++v) {
        if (visited[v]) res.source_side.push_back(v);
        else res.sink_side.push_back(v);
    }
    long long cut_cap = 0;
    for (int u = 0; u < n; ++u) {
        if (!visited[u]) continue;
        for (int idx = g.row_ptr[u]; idx < g.row_ptr[u + 1]; ++idx) {
            int v = g.col_idx[idx];
            if (!visited[v]) {
                long long c = static_cast<long long>(std::llround(g.values[idx]));
                cut_cap += c;
                res.cut_edges.push_back({u, v, c});
            }
        }
    }
    res.min_cut_capacity = cut_cap;

    return res;
}
