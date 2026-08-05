#include "dfs.h"
#include <stack>
#include <vector>

DFSResult dfs(const CSRGraph &g, int source) {
    DFSResult result;
    result.traversal.reserve(g.V);

    std::vector<char> visited(g.V, 0);
    std::stack<int> st;
    st.push(source);

    while (!st.empty()) {
        int u = st.top();
        st.pop();

        if (visited[u]) continue;
        visited[u] = 1;
        result.traversal.push_back(u);

        // Push neighbors in REVERSE order so that, since it's a stack,
        // they get popped/visited in the SAME order they appear in the
        // input file -- matching what a recursive DFS would produce.
        for (int e = g.row_ptr[u + 1] - 1; e >= g.row_ptr[u]; --e) {
            int v = g.col_idx[e];
            if (!visited[v]) {
                st.push(v);
            }
        }
    }

    return result;
}
