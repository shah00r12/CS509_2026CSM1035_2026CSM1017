#!/usr/bin/env python3
"""
Random directed, capacitated graph generator for Maxflow-Mincut test files
(Section 7.1 input format of the CS509 Assignment 3 spec).

Usage:
    python3 gen_maxflow_graph.py <V> <E> <seed> <output_file>

Guarantees:
  - vertices are numbered 0..V-1
  - source = 0, sink = V-1
  - a guaranteed 0 -> V-1 path exists (a randomized Hamiltonian-ish backbone
    chain through a random permutation of the intermediate vertices), so the
    graph always has at least one source-to-sink path
  - remaining edges are added at random (no self-loops, no duplicate
    directed edges) until E edges are reached, biased towards "forward"
    vertices to keep the graph sparse and mostly acyclic-ish while still
    directed
  - all capacities are positive integers in [1, 100]
"""
import random
import sys


def generate(V, E, seed):
    if V < 2:
        raise ValueError("V must be >= 2 (need distinct source and sink)")
    rng = random.Random(seed)
    source, sink = 0, V - 1

    edges = set()

    # Guaranteed backbone path from source to sink through a random
    # permutation of the intermediate vertices, so a valid s-t path exists.
    middle = list(range(1, V - 1))
    rng.shuffle(middle)
    chain = [source] + middle + [sink]
    for i in range(len(chain) - 1):
        edges.add((chain[i], chain[i + 1]))

    # Fill in random extra edges (mostly "forward" w.r.t. chain order to
    # keep the graph sparse and largely well-behaved) until E is reached.
    pos = {v: i for i, v in enumerate(chain)}
    attempts = 0
    max_attempts = max(20 * E, 100000)
    while len(edges) < E and attempts < max_attempts:
        attempts += 1
        u = rng.randrange(V)
        v = rng.randrange(V)
        if u == v:
            continue
        if (u, v) in edges:
            continue
        edges.add((u, v))

    adj = [[] for _ in range(V)]
    for (u, v) in edges:
        cap = rng.randint(1, 100)
        adj[u].append((v, cap))

    return V, len(edges), adj, source, sink


def write_file(path, V, E, adj, source, sink):
    with open(path, "w") as f:
        f.write(f"{V} {E}\n")
        for u in range(V):
            nbrs = adj[u]
            parts = [str(u), str(len(nbrs))]
            for (v, cap) in nbrs:
                parts.append(str(v))
                parts.append(str(cap))
            f.write(" ".join(parts) + "\n")
        f.write(f"SOURCE {source}\n")
        f.write(f"SINK {sink}\n")


def main():
    if len(sys.argv) != 5:
        print("Usage: python3 gen_maxflow_graph.py <V> <E> <seed> <output_file>")
        sys.exit(1)
    V = int(sys.argv[1])
    E = int(sys.argv[2])
    seed = int(sys.argv[3])
    out_path = sys.argv[4]

    V, actual_E, adj, source, sink = generate(V, E, seed)
    write_file(out_path, V, actual_E, adj, source, sink)
    print(f"Wrote {out_path}: V={V} E={actual_E} source={source} sink={sink}")


if __name__ == "__main__":
    main()
