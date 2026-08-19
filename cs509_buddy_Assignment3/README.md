# CS509 – Assignment 3 (Buddy Task)

Algorithms: **Gradient Descent** and **Maxflow-Mincut** (Dinic's algorithm). Maxflow-Mincut
uses a directed, capacitated CSR graph as input; Gradient Descent reads its
polynomial/optimization parameters directly from a text file and does not use CSR.

## 1. Project Structure

```
Assignment3(buddy)/
├── Makefile
├── README.md
├── include/
│   ├── csr_graph.h              # reused from Assignment 1 / 2, unchanged
│   ├── gradient_descent.h
│   └── maxflow_mincut.h
├── src/
│   ├── main.cpp
│   ├── csr_graph.cpp             # reused from Assignment 1 / 2, unchanged
│   ├── gradient_descent.cpp
│   └── maxflow_mincut.cpp
├── tools/
│   └── gen_maxflow_graph.py      # random directed capacitated graph generator
├── tests/
│   ├── gd_01.txt ... gd_05.txt
│   └── maxflow_10.txt ... maxflow_50000.txt
└── build/
    └── assignment3                # compiled binary
```

## 2. Build

```
make            # builds build/assignment3
make clean      # removes build/
```

Compiler flags: `g++ -O2 -std=c++17 -Iinclude -Wall`.

## 3. Run

```
./build/assignment3 gradientdescent <input_file>
./build/assignment3 maxflow         <input_file>
```

Example:

```
./build/assignment3 gradientdescent tests/gd_03.txt
./build/assignment3 maxflow         tests/maxflow_10.txt
```

## 4. Algorithms

### 4.1 Gradient Descent
A single generic one-variable polynomial representation is used for every
required degree: `f(x) = c0 + c1*x + ... + cd*x^d`, with `f'(x)` evaluated
from the same coefficient array. Both `f` and `f'` are evaluated with
Horner's method so the same code path handles all five required degrees (2,
4, 6, 8, 10) without hard-coding a separate function per test case. The
update rule is `x_new = x - learning_rate * f'(x)`; the loop stops when
`|f'(x)| <= tolerance` or `MAX_ITERATIONS` is reached, whichever comes
first. Gradient Descent does not use CSR — its input (degree, coefficients,
initial x, learning rate, tolerance, max iterations) is read directly from
the text file, per the spec's exemption in Section 4.1.

### 4.2 Maxflow-Mincut (Dinic's Algorithm)
Dinic's algorithm was chosen, as recommended by the spec, because it comfortably
handles the largest required graph size (50,000 vertices). The residual
network is built from the already-prepared CSR (each CSR edge becomes a
forward residual edge plus a zero-capacity reverse edge); this construction
is inside the timed algorithm call, as allowed by the spec. Each phase runs
a BFS to build a level graph, then repeated blocking-flow DFS calls (with
per-vertex iterator pointers) push flow along admissible edges until no
augmenting path remains. After the flow is maximal, a BFS/DFS over the final
residual graph (only over edges with remaining residual capacity > 0) finds
every vertex still reachable from the source; that set is the source side of
the minimum cut, its complement is the sink side, and the cut edges are the
original CSR edges crossing from the source side to the sink side. This
residual-graph reachability extraction is also inside the timed section, per
Section 8 of the spec.

## 5. CSR Conversion & Timing Methodology

- Maxflow-Mincut input files are read and parsed into an adjacency list,
  which is then converted to CSR via `adjlist_to_csr` (reused from
  Assignment 1 / 2, not re-implemented, per the spec's instruction).
- The timer starts **only after** the CSR representation is ready (for
  Maxflow-Mincut) or after the input file has been fully read and validated
  (for Gradient Descent), and stops **immediately after** the algorithm call
  returns.
- File reading, input parsing, adjacency-list-to-CSR conversion, and result
  printing are excluded from the reported time, per the assignment's timing
  rule. Residual-network initialization from CSR and min-cut extraction are
  intentionally *included* in the Maxflow-Mincut timing, per Section 8.
- Times are reported in milliseconds from a single run per test file (all
  runs completed well above the timer resolution, so no averaging over
  repeated runs was needed).

## 6. Correctness Verification

- **Gradient Descent**: all five required test cases (`gd_01`–`gd_05`)
  converge to the expected minimum `x* = 3` (for `gd_01`) or `x* = 0` (for
  `gd_02`–`gd_05`), matching the "Expected x*" column in Section 4.3 of the
  spec, with `f(x*) ≈ 0` in every case.
- **Maxflow-Mincut**: verified against the assignment's own worked example
  (Section 7.2/7.3, 6-vertex graph) — the program reproduces the maximum
  flow (23), the minimum cut capacity (23), the source/sink side partition,
  and the exact cut-edge list from the spec.

## 7. Test Graph Generation

Maxflow-Mincut test graphs were generated with
`tools/gen_maxflow_graph.py <V> <E> <seed> <output_file>`, which produces a
random, simple (no self-loops, no duplicate directed edges) directed graph
with the requested vertex/edge count. Source is fixed at vertex 0 and sink
at vertex V-1, and a randomized backbone path from source to sink is always
included first, guaranteeing at least one valid source-to-sink path before
the remaining random edges are added. Capacities are random positive
integers in [1, 100]. Graphs were kept sparse (E on the order of 2.5V–3V) at
every scale, consistent with Section 4.2 of the spec.

Gradient Descent test files (`gd_01.txt`–`gd_05.txt`) use the exact
functions and parameters specified in Section 4.3 of the assignment
(degrees 2, 4, 6, 8, 10).

## 8. Required Report File with Result Tables

### 8.1 Gradient Descent Results Table

| File | Degree | x0 | Rate | Tol. | Max Iter. | Exp. x* | Exp. f(x*) | Actual x | Actual f(x) | Iterations | Time | Status |
|---|---|---|---|---|---|---|---|---|---|---|---|---|
| gd_01.txt | 2 | 0 | 0.10 | 1e-6 | 5,000 | 3 | 0 | 3.000000 | 0.000000 | 70 | 0.001 ms | Pass |
| gd_02.txt | 4 | 2 | 0.02 | 1e-6 | 10,000 | 0 | 0 | 0.000000 | 0.000000 | 180 | 0.002 ms | Pass |
| gd_03.txt | 6 | 2 | 0.02 | 1e-6 | 20,000 | 0 | 0 | 0.000000 | 0.000000 | 349 | 0.006 ms | Pass |
| gd_04.txt | 8 | 2 | 0.01 | 1e-8 | 50,000 | 0 | 0 | 0.000000 | 0.000000 | 948 | 0.021 ms | Pass |
| gd_05.txt | 10 | 2 | 0.005 | 1e-10 | 100,000 | 0 | 0 | 0.000000 | 0.000000 | 2,364 | 0.063 ms | Pass |

All five runs report `Converged: true`.

### 8.2 Maxflow-Mincut Results Table

| File | V | E | Source | Sink | Actual Flow | Cut Capacity | Time | Status |
|---|---|---|---|---|---|---|---|---|
| maxflow_10.txt | 10 | 25 | 0 | 9 | 83 | 83 | 0.013 ms | Pass |
| maxflow_100.txt | 100 | 300 | 0 | 99 | 82 | 82 | 0.169 ms | Pass |
| maxflow_1000.txt | 1,000 | 3,000 | 0 | 999 | 118 | 118 | 1.766 ms | Pass |
| maxflow_10000.txt | 10,000 | 30,000 | 0 | 9,999 | 71 | 71 | 18.212 ms | Pass |
| maxflow_50000.txt | 50,000 | 150,000 | 0 | 49,999 | 173 | 173 | 152.350 ms | Pass |

The 100,000-vertex Maxflow-Mincut case is optional per the spec and was not
required to be run; no test case failed to complete (no core dumps,
out-of-memory, or non-terminating runs) at any of the required sizes.

Notes:
- Maximum flow equals minimum cut capacity in every test, as required.
- "Actual Flow" is intentionally modest relative to graph size because the
  generator's random edges have capacities up to 100 but the guaranteed
  source→sink backbone (through which most flow must ultimately route,
  since the source and sink each start with only a handful of random
  extra edges) bottlenecks the achievable flow — this is expected behavior
  for sparse random capacity graphs and does not indicate a correctness
  issue (confirmed against the spec's own worked example above).

## 9. Spec Worked-Example Verification (raw console output)

**Gradient Descent** (Section 6.2 example, degree 6):
```
Algorithm: Gradient Descent
Degree: 6
Final x: 0.000000
Final f(x): 0.000000
Iterations: 349
Converged: true
Execution time: <value> ms
```

**Maxflow-Mincut** (Section 7.2 example, 6-vertex graph):
```
Algorithm: Maxflow-Mincut
Source: 0
Sink: 5
Maximum flow: 23
Minimum cut capacity: 23
Source side: 0 1 2 4
Sink side: 3 5
Cut edges:
1 3 12
4 3 7
4 5 4
Execution time: <value> ms
```

Both match the assignment PDF's expected output exactly.

## 10. Input Validation

The driver rejects, with a non-zero exit code and a descriptive error
message on stderr, at minimum:
- Missing or unreadable input files.
- Gradient Descent: invalid (negative) degree, a coefficient count different
  from `degree + 1`, a non-positive learning rate, a non-positive tolerance,
  or a non-positive `MAX_ITERATIONS`.
- Maxflow-Mincut: invalid source/sink vertices (out of `[0, V-1]`), source
  equal to sink, or negative capacities.
