# Algorithms

A multi-language algorithms playground organized by topic, with shared test cases and aligned implementations in:

- C
- Java
- Python

The goal of this repository is twofold:
- learn how classic algorithms and data structures work
- compare how the same ideas look across different programming languages

---

## Current status

### Implemented modules

#### 1. Sorting
Implemented in:
- `Sorting/C/`
- `Sorting/Java/`
- `Sorting/Python/`

Includes:
- Bubble sort
- Selection sort
- Insertion sort
- Shell sort
- Merge sort
- Heap sort
- Quick sort
- Radix sort
- Bucket sort
- Standard library baseline sort

Also includes:
- per-language benchmark modes
- cross-language comparison tool
- shared sorting test cases

Main docs:
- `Sorting/README.md`

#### 2. Trees
Implemented in:
- `Trees/C/`
- `Trees/Java/`
- `Trees/Python/`

Includes:
- Trie / Prefix Tree
- Binary Search Tree
- Heap / Priority Queue
- AVL Tree
- Fenwick Tree
- Segment Tree

Also includes:
- shared query-based test cases
- aligned demos across all three languages

Main docs:
- `Trees/README.md`

---

## Scaffolded modules

These topic folders exist and are ready for future work:

- `Searching/`
- `Graphs/`
- `DynamicProgramming/`
- `Strings/`

Each of them follows the intended structure:
- `C/`
- `Java/`
- `Python/`
- `test_case/`
- topic `README.md`

---

## Repository layout

```text
Algorithm/
├── README.md
├── Sorting/
├── Trees/
├── Searching/
├── Graphs/
├── DynamicProgramming/
└── Strings/
```

---

## Design approach

This repository is organized by **topic first**, then by **language**.

That means each module can contain:
- topic-specific implementations
- topic-specific test cases
- topic-specific benchmarks or demos
- language-by-language comparisons

This structure scales better than mixing every algorithm into a single folder.

---

## How to use the repo

### If you want to study one topic
Open the topic README first:
- `Sorting/README.md`
- `Trees/README.md`

These explain:
- what is implemented
- how each algorithm or structure works
- how to run the demos
- how to compare behavior across languages

### If you want to compare languages
Use the topic implementations side by side.

For example:
- `Sorting/` includes benchmark tooling
- `Trees/` includes aligned query-driven demos for C, Java, and Python

---

## Suggested learning path

A good progression through the current repo is:

1. Sorting
   - start with bubble, selection, insertion
   - then move to merge, quick, heap
   - then radix and bucket sort

2. Trees
   - Binary Search Tree
   - Heap / Priority Queue
   - Trie / Prefix Tree
   - AVL Tree
   - Fenwick Tree
   - Segment Tree

Then move on to future modules like:
- Searching
- Graphs
- Dynamic Programming
- Strings

---

## Suggested next expansions

Good next topics to implement:

### Searching
- Linear search
- Binary search
- Jump search
- Interpolation search
- Exponential search

### Graphs
- BFS
- DFS
- Dijkstra
- Topological sort
- Union-Find
- Kruskal
- Prim

### Dynamic Programming
- Fibonacci
- Coin change
- 0/1 Knapsack
- Longest common subsequence
- Edit distance

### Strings
- KMP
- Rabin-Karp
- Z algorithm
- Boyer-Moore
- Suffix array

---

## Philosophy

This repo is meant to be both:
- a reference for how algorithms work
- and a practical codebase for comparing implementations across languages

So each topic should aim to include:
- clear code
- shared test cases
- readable documentation
- comparable behavior across languages
