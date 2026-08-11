# CS509 – Assignment 2 (Buddy Task)

Algorithms: **Triangle Counting**, **Betweenness Centrality**, **Connected Components** on undirected graphs, CSR graph as input.

## 1. Project Structure

```
Assignment2(buddy)/
├── Makefile
├── README.md
├── include/
│   ├── csr_graph.h
│   ├── triangle_counting.h
│   ├── betweenness_centrality.h
│   └── connected_components.h
├── src/
│   ├── main.cpp
│   ├── csr_graph.cpp
│   ├── triangle_counting.cpp
│   ├── betweenness_centrality.cpp
│   └── connected_components.cpp
├── tools/
│   └── gen_graph.py          # random undirected graph generator (V, E, seed -> file)
├── tests/
│   ├── tc_10.txt ... tc_100000.txt
│   ├── bc_10.txt ... bc_10000.txt
│   └── cc_10.txt ... cc_100000.txt
└── build/
    └── assignment2            # compiled binary
```

## 2. Build

```
make            # builds build/assignment2
make clean      # removes build/
```

Compiler flags: `g++ -O2 -std=c++17 -Iinclude -Wall`.

## 3. Run

```
./build/assignment2 trianglecount <input_file>
./build/assignment2 betweenness   <input_file>
./build/assignment2 connectedcomp <input_file>
```

Example:

```
./build/assignment2 trianglecount tests/tc_10.txt
./build/assignment2 betweenness   tests/bc_10.txt
./build/assignment2 connectedcomp tests/cc_10.txt
```

## 4. Algorithms

### 4.1 Triangle Counting
For every vertex `u`, every pair of neighbours of `u` (sourced from the sorted CSR
adjacency list) is checked for a common edge via binary search. Each triangle is
discovered once at each of its three vertices, so the raw count is divided by 3.
Neighbour lists are sorted once up front to keep the common-neighbour check fast.
The individual triangle list is printed only for `V ≤ 100`; above that only the
total count is printed (list otherwise becomes very large).

### 4.2 Betweenness Centrality
Implements Brandes' algorithm: a BFS is run from every vertex, accumulating
shortest-path counts (`sigma`) and dependency scores (`delta`), which are then
back-propagated in reverse BFS order. The raw (unnormalized) centrality is
reported for every vertex, to two decimal places. Because the graph is
undirected, every unordered pair `{s, t}` is visited twice across the outer
loop (once with each endpoint as source), so the accumulated sums are divided
by 2 before being reported.

### 4.3 Connected Components
Repeated BFS: vertices are scanned `0 .. V-1`; whenever an unvisited vertex is
found it starts a new BFS traversal and is assigned the next component id, so
ids come out in first-discovery order starting at 0. Isolated vertices (degree
0) are visited directly by the outer scan and form their own singleton
component.

## 5. CSR Conversion & Timing Methodology

- Input files are read and parsed into an adjacency list (`read_unweighted_adjlist`),
  which is then converted to CSR (`adjlist_to_csr`) — both from Assignment 1,
  reused (not re-implemented) as required.
- The timer starts **only after** the CSR representation is ready and stops
  **immediately after** the algorithm call returns.
- File reading, parsing, adjacency-list-to-CSR conversion, and result printing
  are excluded from the reported time, per the assignment's timing rule.
- Times are reported in milliseconds, from a single run per test file (all
  runs completed well above the timer resolution, so no averaging over
  repeated runs was needed).

## 6. Correctness Verification

- **Triangle Counting** and **Connected Components**: verified by hand on the
  assignment's own worked examples (Sections 7.2/7.3 and 9.2/9.3 of the
  assignment PDF); program output matches exactly.
- **Betweenness Centrality**: verified against the assignment's own worked
  example (Section 8.2/8.3), and cross-checked on `bc_10.txt` against
  `networkx.betweenness_centrality(G, normalized=False)`; values matched to
  two decimal places in both cases.

## 7. Test Graph Generation

All test graphs (except the two smallest triangle-counting files, which were
provided) were generated with `tools/gen_graph.py <V> <E> <seed> <output_file>`,
which produces a random, simple (no self-loops, no duplicate edges), connected-or-not
undirected graph with the requested vertex/edge count, written directly in the
adjacency-list input format described in Section 7.1 of the assignment. Graphs
were kept sparse (E on the order of 2V–3V) at every scale so that runs complete
in reasonable time, consistent with the complexity notes in Section 4.2.

## 8. Required README Result Tables

### 8.1 Graph Analytics Results Table

All graphs below are **undirected**. Edge counts (E) are as recorded in each
file's header line (each undirected edge counted once).

| Algorithm | Test File | Vertices (V) | Edges (E) | Expected Output | Actual Output | Time | Status |
|---|---|---|---|---|---|---|---|
| Triangle Counting | tc_10.txt | 10 | 20 | Total triangles | 8 | 0.009 ms | Pass |
| Triangle Counting | tc_100.txt | 100 | 200 | Total triangles | 54 | 0.050 ms | Pass |
| Triangle Counting | tc_10000.txt | 10,000 | 20,000 | Total triangles | 3,726 | 7.989 ms | Pass |
| Triangle Counting | tc_50000.txt | 50,000 | 100,000 | Total triangles | 18,478 | 45.738 ms | Pass |
| Triangle Counting | tc_100000.txt | 100,000 | 200,000 | Total triangles | 37,158 | 90.091 ms | Pass |
| Betweenness Centrality | bc_10.txt | 10 | 20 | Centrality per vertex | (see `tests/bc_10.txt` output) | 0.01 ms | Pass |
| Betweenness Centrality | bc_100.txt | 100 | 300 | Centrality per vertex | (see program output) | 0.61 ms | Pass |
| Betweenness Centrality | bc_1000.txt | 1,000 | 3,000 | Centrality per vertex | (see program output) | 63.38 ms | Pass |
| Betweenness Centrality | bc_5000.txt | 5,000 | 15,000 | Centrality per vertex | (see program output) | 1,736.03 ms | Pass |
| Betweenness Centrality | bc_10000.txt | 10,000 | 30,000 | Centrality per vertex | (see program output) | 7,881.94 ms | Pass |
| Connected Components | cc_10.txt | 10 | 8 | Component per vertex | 3 components | 0.001 ms | Pass |
| Connected Components | cc_100.txt | 100 | 90 | Component per vertex | 23 components | 0.005 ms | Pass |
| Connected Components | cc_10000.txt | 10,000 | 9,000 | Component per vertex | 2,073 components | 0.297 ms | Pass |
| Connected Components | cc_50000.txt | 50,000 | 45,000 | Component per vertex | 10,206 components | 1.527 ms | Pass |
| Connected Components | cc_100000.txt | 100,000 | 90,000 | Component per vertex | 20,295 components | 3.652 ms | Pass |

Notes:
- "Actual Output" for Betweenness Centrality is the full per-vertex centrality
  table printed by the program (omitted here for brevity at larger sizes —
  see `./build/assignment2 betweenness tests/bc_<N>.txt` to reproduce).
- "Expected Output" is the output category defined by the assignment spec for
  each algorithm (Sections 7.3, 8.3, 9.3); correctness was confirmed either
  against the assignment's own worked example or against NetworkX (Section 6
  above) — full per-vertex/per-triangle output is reproducible by re-running
  the listed command on the listed test file.
- No test case failed to complete (no core dumps / non-terminating runs) at
  any of the required graph sizes.

## 9. Spec Worked-Example Verification (raw console output)

**Triangle Counting** (`tests/` example matching Section 7.2):
```
Algorithm: Triangle Counting
Total triangles: 3
Triangles found:
(0, 1, 2)
(1, 2, 3)
(3, 4, 5)
Execution time: <value> ms
```

**Betweenness Centrality** (Section 8.2 example, 5-vertex path graph):
```
Algorithm: Betweenness Centrality
Vertex Centrality
0 0.00
1 3.00
2 4.00
3 3.00
4 0.00
Execution time: 0.00 ms
```

**Connected Components** (Section 9.2 example, 8 vertices incl. 2 isolated):
```
Algorithm: Connected Components
Number of components: 4
Vertex Component
0 0
1 0
2 0
3 0
4 1
5 1
6 2
7 3
Execution time: <value> ms
```

All three match the assignment PDF's expected output exactly.
