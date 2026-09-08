# CS509 — FastMap and K-Means Clustering

**Submitted by:**
Shahoor Ahmed — Entry No. 2026CSM1035
Sudip Mondal — Entry No. 2026CSM1017

## Overview

This project implements two algorithms in C++:

- **FastMap** — embeds a set of N objects, described only by an N×N pairwise
  distance matrix, into a K-dimensional Euclidean space by iteratively picking
  a pair of far-apart "pivot" objects and projecting every object onto the
  line joining them.
- **K-Means Clustering** — partitions N points of dimension D into K clusters
  by alternating an assignment step (nearest centroid) and an update step
  (recompute centroids), minimizing within-cluster sum of squares (WCSS).

Both algorithms are driven from a single executable that reads a plain-text
input file, runs the requested algorithm, times it, and prints the results.

## Project Structure

```
.
├── driver/
│   └── main.cpp          # CLI entry point, file parsing, timing, output
├── source/
│   ├── fastmap.h / .cpp  # FastMap implementation
│   └── kmeans.h  / .cpp  # K-Means implementation
├── testfiles/
│   ├── fm_test_small.txt, fm_01.txt … fm_04.txt   # FastMap inputs
│   └── km_test_small.txt, km_01.txt … km_04.txt   # K-Means inputs
├── Makefile
└── README.md
```

## Building

```bash
make
```

This compiles `driver/main.cpp`, `source/kmeans.cpp`, and `source/fastmap.cpp`
into `main.exe`. To rebuild from a clean state:

```bash
make clean
make
```

> Note: the provided `Makefile`'s `clean` target uses Windows `del`. On
> Linux/macOS, clean manually instead:
> `rm -f driver/main.o source/*.o main.exe`

Alternatively, compile directly with g++:

```bash
g++ -O2 -std=c++17 -Isource driver/main.cpp source/kmeans.cpp source/fastmap.cpp -o main.exe
```

## Usage

```
main <algorithm> <input_file>
```

- `algorithm` — `kmeans` or `fastmap`
- `input_file` — path to a correctly formatted input file (see below)

Examples:

```bash
./main.exe kmeans testfiles/km_01.txt
./main.exe fastmap testfiles/fm_01.txt
```

## Input File Formats

**K-Means** (`km_*.txt`):

```
N D K
<N lines of D space-separated coordinates>
MAX_ITERATIONS <int>
TOLERANCE <double>
```

**FastMap** (`fm_*.txt`):

```
N K
<N lines of N space-separated values — the symmetric distance matrix>
```

The distance matrix must be square, symmetric, non-negative, and have a zero
diagonal; `K` must satisfy `0 < K < N`. Malformed files are rejected with an
`Invalid ... input file.` message rather than causing a crash.

## Output

- **K-Means**: per-point cluster assignment, final centroids, WCSS, number of
  iterations run, whether the run converged (centroid movement fell below
  `TOLERANCE`), and execution time in milliseconds.
- **FastMap**: the pivot pair chosen for each of the K dimensions, then the
  K-dimensional coordinates for every object, and execution time in
  milliseconds.

## Test Results Summary

All test files were run against the corresponding algorithm after a clean
rebuild (g++, C++17, `-O2`, no warnings).

### K-Means

| Test file          | N       | D | K  | Iterations | Converged | WCSS          | Time (ms) |
|---------------------|---------|---|----|------------|-----------|---------------|-----------|
| km_test_small.txt   | 6       | 2 | 2  | 3          | Yes       | 7.875000      | 0.003720  |
| km_01.txt           | 100     | 2 | 3  | 2          | Yes       | 254.515655    | 0.005087  |
| km_02.txt           | 1,000   | 2 | 5  | 2          | Yes       | 2600.164175   | 0.031470  |
| km_03.txt           | 10,000  | 5 | 8  | 17         | Yes       | 59111.952110  | 7.087245  |
| km_04.txt           | 100,000 | 5 | 10 | 20         | Yes       | 567405.427429 | 99.275056 |

All five runs converged well within the 300-iteration cap. Runtime scales
roughly with N·K·D per iteration, as expected.

### FastMap

| Test file          | N (declared) | K | Status                        | Time (ms) |
|---------------------|--------------|---|--------------------------------|-----------|
| fm_test_small.txt   | 5            | 2 | Valid — ran                   | 0.002145  |
| fm_01.txt           | 10           | 2 | Valid — ran                   | 0.003475  |
| fm_02.txt           | 100          | 2 | Valid — ran                   | 0.078444  |
| fm_03.txt           | 1,000        | 3 | Valid — ran                   | 11.797901 |
| fm_04.txt           | 10,000       | 3 | Rejected as invalid (see note) | —         |

Pivot pairs chosen:
- fm_test_small.txt — Dim 1: (4, 0), Dim 2: (2, 1)
- fm_01.txt — Dim 1: (6, 0), Dim 2: (9, 8)
- fm_02.txt — Dim 1: (69, 54), Dim 2: (14, 12)
- fm_03.txt — Dim 1: (229, 404), Dim 2: (878, 90), Dim 3: (210, 816)

**Note on fm_04.txt:** the driver correctly reports this file as invalid. Its
header declares `N = 10000` (a 10000×10000 matrix), but the file only
contains 5,479 data rows before it ends — a truncated/incomplete test file,
not a code defect. `readFastMapFile()` detects the stream running out of data
partway through the matrix and returns `false` instead of reading garbage or
crashing, exactly as designed. FastMap's cost scales roughly O(N²), consistent
with the ~100× runtime increase observed going from N=100 to N=1,000.

## Requirements

- C++17-compatible compiler (tested with g++ 13.3.0)
- No external dependencies beyond the C++ standard library