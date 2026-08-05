# Assignment 1 — Part 2 (Buddy Task): BFS, DFS, SSSP

Builds on the CSR module from Part 1 (`csr_graph.h`/`csr_graph.cpp` are
copied in unchanged) — that's exactly the shared infrastructure it was
designed for.

## Build

**Mac/Linux:**
```
make
```
Produces `build/assignment1_part2`.

**Windows (VS Code):** `.vscode/tasks.json` / `launch.json` are already set
up — see "Running on Windows in VS Code" below. Requires MinGW g++/gdb on
PATH (same setup as Part 1).

## Run

```
./build/assignment1_part2 bfs  <input_file>
./build/assignment1_part2 dfs  <input_file>
./build/assignment1_part2 sssp <input_file>
```
(Windows: `.\assignment1_part2.exe bfs tests\bfs_10.txt`, etc.)

## Design notes

- **BFS** (`src/bfs.cpp`) — standard queue-based BFS directly over the CSR
  arrays (`row_ptr`/`col_idx`). Reports min edge-count distance to every
  vertex; unreachable vertices print as `INF`.
- **DFS** (`src/dfs.cpp`) — **iterative** (explicit `std::stack`), not
  recursive, so it's safe on the 100,000-vertex test without risking a
  call-stack overflow. Neighbors are pushed in reverse order so they pop
  off in the same order they're listed in the input file — this matches
  what a straightforward recursive DFS would produce, and is reproducible
  for a given file.
- **SSSP** (`src/sssp.cpp`) — Dijkstra's algorithm with a binary min-heap
  (`std::priority_queue`), O((V+E) log V). Positive weights only, per the
  assignment's requirement. Stale heap entries are skipped via a
  `finalized[]` check rather than a decrease-key, which is the standard
  simple approach with `std::priority_queue`.
- **Timing discipline**: in `main.cpp`, file reading, adjacency-list
  parsing, and `adjlist_to_csr` conversion all happen *before* the
  `Clock::now()` calls — only the algorithm call itself is timed, per the
  assignment's rule.

## Test graphs

`scripts/generate_graphs.py` generates one connected random graph per
required size (10 / 100 / 10,000 / 50,000 / 100,000 vertices, average
degree 4) via a random spanning tree (guarantees every vertex is reachable
from `SOURCE 0`) plus extra random edges. The same underlying graph is
written out three times: `bfs_V.txt` / `dfs_V.txt` (unweighted) and
`sssp_V.txt` (same edges, random positive integer weights 1–20).

Two hand-written files reproduce the spec's own worked examples exactly,
for a black-box sanity check:
- `tests/bfs_dfs_example.txt` — spec section 6.2
- `tests/sssp_example.txt` — spec section 7.2

To regenerate the graphs (e.g. with a different seed or average degree):
```
python3 scripts/generate_graphs.py
```

## Verification against the spec's worked examples

```
./build/assignment1_part2 bfs  tests/bfs_dfs_example.txt
./build/assignment1_part2 dfs  tests/bfs_dfs_example.txt
./build/assignment1_part2 sssp tests/sssp_example.txt
```

| Algorithm | Expected (spec) | Actual (this program) |
|---|---|---|
| BFS traversal | `0 1 2 3 4` | `0 1 2 3 4` ✅ |
| BFS distances | `0:0 1:1 2:1 3:2 4:2` | `0:0 1:1 2:1 3:2 4:2` ✅ |
| DFS traversal | `0 1 3 2 4` | `0 1 3 2 4` ✅ |
| SSSP distances | `0:0 1:3 2:1 3:4 4:7` | `0:0 1:3 2:1 3:4 4:7` ✅ |

All four match exactly.

## 9.2 Graph Results Table

| Algorithm | Test File | Vertices | Edges | Input Type | Source | Expected Output | Actual Output | Time | Status |
|---|---|---|---|---|---|---|---|---|---|
| BFS | bfs_10.txt | 10 | 20 | Unweighted adjacency list | 0 | traversal + distances | (see run) | 0.00083 ms | Pass |
| BFS | bfs_100.txt | 100 | 200 | Unweighted adjacency list | 0 | traversal + distances | (see run) | 0.00394 ms | Pass |
| BFS | bfs_10000.txt | 10000 | 20000 | Unweighted adjacency list | 0 | traversal + distances | (see run) | 0.376 ms | Pass |
| BFS | bfs_50000.txt | 50000 | 100000 | Unweighted adjacency list | 0 | traversal + distances | (see run) | 2.153 ms | Pass |
| BFS | bfs_100000.txt | 100000 | 200000 | Unweighted adjacency list | 0 | traversal + distances | (see run) | 4.853 ms | Pass |
| DFS | dfs_10.txt | 10 | 20 | Unweighted adjacency list | 0 | valid traversal | (see run) | 0.00125 ms | Pass |
| DFS | dfs_100.txt | 100 | 200 | Unweighted adjacency list | 0 | valid traversal | (see run) | 0.00521 ms | Pass |
| DFS | dfs_10000.txt | 10000 | 20000 | Unweighted adjacency list | 0 | valid traversal | (see run) | 0.499 ms | Pass |
| DFS | dfs_50000.txt | 50000 | 100000 | Unweighted adjacency list | 0 | valid traversal | (see run) | 2.938 ms | Pass |
| DFS | dfs_100000.txt | 100000 | 200000 | Unweighted adjacency list | 0 | valid traversal | (see run) | 6.574 ms | Pass |
| SSSP | sssp_10.txt | 10 | 20 | Positive weighted adjacency list | 0 | shortest distances | (see run) | 0.00256 ms | Pass |
| SSSP | sssp_100.txt | 100 | 200 | Positive weighted adjacency list | 0 | shortest distances | (see run) | 0.02161 ms | Pass |
| SSSP | sssp_10000.txt | 10000 | 20000 | Positive weighted adjacency list | 0 | shortest distances | (see run) | 2.409 ms | Pass |
| SSSP | sssp_50000.txt | 50000 | 100000 | Positive weighted adjacency list | 0 | shortest distances | (see run) | 14.308 ms | Pass |
| SSSP | sssp_100000.txt | 100000 | 200000 | Positive weighted adjacency list | 0 | shortest distances | (see run) | 32.775 ms | Pass |

> Timings above were measured on the machine used to prepare this
> solution. Re-run each command on your own grading machine and replace
> these with your actual numbers before submitting. "Status: Pass" here
> means: (a) the small examples matched the spec's worked answers exactly,
> and (b) every vertex in the generated graphs was confirmed reachable
> (no unexpected `INF`) and DFS visited each vertex exactly once — see the
> "Verification" section above and the sanity checks run during
> development.

## Running on Windows in VS Code

Same setup as Part 1 (MSYS2 / MinGW-w64 g++ + gdb on PATH). Full
step-by-step instructions are in Part 1's README; the short version:

1. Install MSYS2, then in the "MSYS2 UCRT64" terminal:
   `pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-gdb`
2. Add `C:\msys64\ucrt64\bin` to your Windows PATH, open a fresh terminal,
   confirm `g++ --version` and `gdb --version` work.
3. **File → Open Folder...** this `assignment1_part2` folder in VS Code.
4. Install the **C/C++** extension (Microsoft) if you don't have it.
5. **Ctrl+Shift+B** to build (`assignment1_part2.exe` appears in the
   project root).
6. Run from the built-in terminal:
   ```
   .\assignment1_part2.exe bfs tests\bfs_10.txt
   ```
7. To debug: set a breakpoint in `src/main.cpp`, open Run and Debug
   (Ctrl+Shift+D), pick one of the configurations in `.vscode/launch.json`,
   press **F5**.

## Files

```
include/csr_graph.h     (from Part 1) CSR struct + adjacency-list readers
include/bfs.h            BFS declarations
include/dfs.h            DFS declarations
include/sssp.h           SSSP declarations
src/csr_graph.cpp        (from Part 1) adjacency-list-to-CSR converter
src/bfs.cpp              Queue-based BFS
src/dfs.cpp              Iterative (stack-based) DFS
src/sssp.cpp             Dijkstra's algorithm (min-heap)
src/main.cpp             Driver (CLI: bfs / dfs / sssp modes)
scripts/generate_graphs.py   Generates the 5 required test-graph sizes
tests/bfs_dfs_example.txt    Spec's worked example (section 6.2)
tests/sssp_example.txt       Spec's worked example (section 7.2)
tests/{bfs,dfs,sssp}_{10,100,10000,50000,100000}.txt   Required size sweep
.vscode/tasks.json, launch.json   VS Code build/debug config (Windows-ready)
Makefile
```
