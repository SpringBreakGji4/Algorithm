# Sorting Module

This module contains classic sorting algorithms implemented in C, Java, and Python, along with benchmarking and cross-language comparison tools.

## Implemented algorithms

- Bubble sort
- Selection sort
- Insertion sort
- Shell sort
- Merge sort
- Heap sort
- Quick sort
- Radix sort
- Bucket sort
- Standard library baseline
  - C: `qsort`
  - Java: `Arrays.sort`
  - Python: `list.sort`

Implemented in:
- `C/`
- `Java/`
- `Python/`

Shared input files live in:
- `test_case/`

Cross-language comparison tool:
- `compare_benchmarks.py`

---

## Folder layout

```text
Sorting/
├── C/
├── Java/
├── Python/
├── test_case/
├── compare_benchmarks.py
├── generate_benchmark_chart.py
├── results/                # generated when chart workflow runs
└── README.md
```

---

## How each sorting algorithm works

## Visual map of sorting families

```text
Sorting
├── Comparison-based
│   ├── Quadratic / simple
│   │   ├── Bubble sort
│   │   ├── Selection sort
│   │   └── Insertion sort
│   ├── Gap-based
│   │   └── Shell sort
│   └── O(n log n)-oriented
│       ├── Merge sort
│       ├── Heap sort
│       └── Quick sort
└── Non-comparison / distribution-based
    ├── Radix sort
    └── Bucket sort
```

## Visual intuition

### 1. Bubble Sort

Bubble sort repeatedly compares adjacent elements and swaps them if they are out of order.

#### ASCII view

```text
Pass 1:
[5 1 4 2 8]
 5>1 swap  -> [1 5 4 2 8]
 5>4 swap  -> [1 4 5 2 8]
 5>2 swap  -> [1 4 2 5 8]
 5<8 keep  -> [1 4 2 5 8]
                    ^
          largest value bubbled right
```

#### Core idea
After each full pass through the array, the largest remaining unsorted element “bubbles” to the end.

#### Why it matters
- very easy to understand
- good as a first sorting algorithm
- useful for demonstrating why asymptotic complexity matters

#### Typical complexity
- best case: `O(n)` with early-exit optimization
- average case: `O(n^2)`
- worst case: `O(n^2)`
- space: `O(1)`

#### Notes
- stable
- very slow on large inputs

---

### 2. Selection Sort

Selection sort repeatedly selects the smallest remaining element and places it at the next sorted position.

#### ASCII view

```text
Start:   [64 25 12 22 11]
Pick min from whole range ------> 11
Swap into front: [11 25 12 22 64]
                  ^
             sorted boundary grows rightward
```

#### Core idea
At step `i`, scan the unsorted portion, find the minimum, and swap it into index `i`.

#### Why it matters
- simple to implement
- useful for understanding in-place selection
- performs a predictable number of comparisons

#### Typical complexity
- best case: `O(n^2)`
- average case: `O(n^2)`
- worst case: `O(n^2)`
- space: `O(1)`

#### Notes
- not stable in its typical form
- usually slower than insertion sort on practical data

---

### 3. Insertion Sort

Insertion sort builds the sorted array one element at a time.

#### ASCII view

```text
Sorted | Unsorted
[3 7 9 | 5 2]
       take 5
[3 7 9 | 7 2]
[3 7 7 | 9 2]
[3 5 7 9 | 2]
```

#### Core idea
Take the next element and insert it into the correct position in the already-sorted left side.

#### Why it matters
- efficient for small inputs
- works well on nearly sorted data
- often used inside more advanced hybrid algorithms

#### Typical complexity
- best case: `O(n)`
- average case: `O(n^2)`
- worst case: `O(n^2)`
- space: `O(1)`

#### Notes
- stable
- much more practical than bubble sort for small datasets

---

### 4. Shell Sort

Shell sort improves insertion sort by comparing elements that are far apart first, then reducing the gap.

#### ASCII view

```text
Array: [9 8 3 7 5 6 4 1]
Gap = 4
Groups:
(9,5) (8,6) (3,4) (7,1)
  ↓     ↓     ↓     ↓
(5,9) (6,8) (3,4) (1,7)

Then reduce gap: 4 -> 2 -> 1
```

#### Core idea
Instead of shifting elements only one position at a time, it performs insertion-sort-like passes over elements separated by a gap.

#### Why it matters
- bridges the gap between simple quadratic sorts and faster `O(n log n)` sorts
- demonstrates how gap sequences improve local ordering

#### Typical complexity
- depends on gap sequence
- often around `O(n log n)` to `O(n^2)` in practice
- space: `O(1)`

#### Notes
- not stable
- usually much faster than bubble/selection/insertion on larger inputs

---

### 5. Merge Sort

Merge sort uses divide and conquer.

#### ASCII view

```text
[38 27 43 3 9 82 10]
        split
[38 27 43] + [3 9 82 10]
   /   \        /     \
...   ...     ...    ...
        merge sorted halves back upward

[27 38 43] + [3 9 10 82]
          -> [3 9 10 27 38 43 82]
```

#### Core idea
- split the array into two halves
- recursively sort each half
- merge the two sorted halves into one sorted array

#### Why it matters
- classic example of divide-and-conquer design
- very consistent performance
- naturally stable

#### Typical complexity
- best case: `O(n log n)`
- average case: `O(n log n)`
- worst case: `O(n log n)`
- space: `O(n)`

#### Notes
- stable
- requires extra memory for merging
- excellent for linked lists and external sorting scenarios too

---

### 6. Heap Sort

Heap sort builds a max-heap, then repeatedly extracts the largest element.

#### ASCII view

```text
Max-heap:
        9
      /   \
     7     6
    / \   / \
   5  3  2  1

Swap root with last:
[1 7 6 5 3 2 | 9]
Re-heapify remaining left side
```

#### Core idea
- first arrange data into a heap
- swap the root with the last element
- shrink the heap
- restore heap order
- repeat

#### Why it matters
- uses heap structure directly
- guarantees `O(n log n)` worst-case time
- in-place sorting method

#### Typical complexity
- best case: `O(n log n)`
- average case: `O(n log n)`
- worst case: `O(n log n)`
- space: `O(1)`

#### Notes
- not stable
- often a bit slower in practice than quick sort, but more predictable

---

### 7. Quick Sort

Quick sort also uses divide and conquer, but partitions around a pivot.

#### ASCII view

```text
[8 3 1 7 0 10 2]
          ^ pivot = 7

Partition:
[3 1 0 2] 7 [8 10]

Then recurse on left and right partitions.
```

#### Core idea
- choose a pivot
- place smaller elements on one side
- place larger elements on the other side
- recursively sort the partitions

#### Why it matters
- one of the most important practical sorting algorithms
- often very fast in real workloads
- highlights how pivot strategy affects performance

#### Typical complexity
- best case: `O(n log n)`
- average case: `O(n log n)`
- worst case: `O(n^2)`
- space: depends on recursion depth

#### Notes
- not stable in the typical in-place form
- current implementations use median-of-three style pivot selection to avoid bad cases more often

---

### 8. Radix Sort

Radix sort is a non-comparison sort.

#### ASCII view

```text
Input: [170, 45, 75, 90, 802, 24, 2, 66]

By ones digit  -> [170, 90, 802, 2, 24, 45, 75, 66]
By tens digit  -> [802, 2, 24, 45, 66, 170, 75, 90]
By hundreds    -> [2, 24, 45, 66, 75, 90, 170, 802]
```

#### Core idea
Instead of comparing whole numbers directly, it sorts by digits one place at a time.
For example:
- ones place
- tens place
- hundreds place

This implementation uses counting sort as the stable subroutine for each digit.

#### Why it matters
- can outperform comparison sorts for suitable numeric data
- demonstrates that not all sorting is based on comparisons

#### Typical complexity
- often expressed as `O(d(n + b))`
  - `d` = number of digits
  - `b` = base / bucket count
- space: `O(n + b)`

#### Notes
- very fast for integers under the right conditions
- performance depends on data representation and range

---

### 9. Bucket Sort

Bucket sort distributes values into buckets, sorts each bucket, then concatenates them.

#### ASCII view

```text
Values: [0.78, 0.17, 0.39, 0.26, 0.72, 0.94, 0.21]

Bucket 0: [0.17]
Bucket 1: [0.21, 0.26]
Bucket 2: [0.39]
Bucket 3: []
Bucket 4: []
Bucket 5: []
Bucket 6: []
Bucket 7: [0.72, 0.78]
Bucket 8: []
Bucket 9: [0.94]

Concatenate buckets in order.
```

#### Core idea
- map each input value to a bucket based on its range
- sort within each bucket
- combine buckets in order

#### Why it matters
- useful when data is spread fairly evenly across a known range
- shows how distribution-based sorting differs from direct comparison sorting

#### Typical complexity
- average case: `O(n + k)` plus cost of sorting buckets
- worst case can degrade badly if everything lands in one bucket
- space: depends on bucket representation

#### Notes
- highly data-dependent
- can be extremely fast when the distribution is favorable

---

### 10. Standard Library Sorts

Each language also includes its built-in sorting baseline.

#### ASCII view

```text
Your code          Library sort
----------   ->   -----------------
raw array         highly optimized runtime implementation

Use custom sorts to learn.
Use library sorts when shipping real products.
```

#### Why this matters
It gives you a practical reference point:
- how close custom algorithms are to optimized library code
- how much language/runtime optimizations matter

Current baselines:
- C: `qsort`
- Java: `Arrays.sort`
- Python: `list.sort`

#### Notes
- these are the real-world defaults most developers would use
- very useful for comparison in benchmarks

---

## Implementations by language

### C
Structure:
- `include/sorting.h`
- `src/algorithms.c`
- `src/utils.c`
- `src/main.c`

Build and run:
```bash
make
make run
./bin/sorting_demo benchmark 5
./bin/sorting_demo benchmark-tsv 5
```

### Java
Structure:
- `src/SortingDemo.java`
- `bin/` for compiled classes

Build and run:
```bash
cd Java
javac -d bin src/SortingDemo.java
java -cp bin SortingDemo all
java -cp bin SortingDemo benchmark 5
java -cp bin SortingDemo benchmark-tsv 5
```

### Python
Structure:
- `sorting_demo.py`

Run:
```bash
cd Python
python3 sorting_demo.py all
python3 sorting_demo.py benchmark 5
python3 sorting_demo.py benchmark-tsv 5
```

---

## Benchmarking and comparison

### Per-language benchmark modes
Each implementation supports:
- running one algorithm
- running all algorithms
- summary benchmark mode
- machine-readable TSV export mode
- repeated runs for more stable timing

### Cross-language comparison
Use the top-level comparison runner:

```bash
python3 compare_benchmarks.py
python3 compare_benchmarks.py 3
```

### Simple benchmark chart workflow
If you want a lightweight artifact you can commit or preview on GitHub, generate a CSV and a Mermaid markdown chart:

```bash
python3 generate_benchmark_chart.py
python3 generate_benchmark_chart.py 5 --mode full
```

This writes:
- `results/benchmark_results.csv`
- `results/benchmark_chart.md`

The markdown report includes Mermaid bar charts for C, Java, and Python plus a ranked table for each language.

#### Modes
Fast mode is the default.
It skips the slow quadratic algorithms:
- bubble sort
- selection sort
- insertion sort

Run everything:
```bash
python3 compare_benchmarks.py --full
```

Run only the slow quadratic sorts:
```bash
python3 compare_benchmarks.py --slow-only
```

The comparison script:
- builds the C and Java demos
- runs C, Java, and Python export mode
- groups results by algorithm
- prints per-algorithm cross-language rankings
- prints a summary matrix of average runtime

---

## Suggested learning order

A good order for learning the sorting algorithms in this module is:

1. Bubble Sort
2. Selection Sort
3. Insertion Sort
4. Merge Sort
5. Quick Sort
6. Heap Sort
7. Shell Sort
8. Radix Sort
9. Bucket Sort
10. Standard library sorts for practical comparison

Why:
- start with the simplest quadratic algorithms
- move into divide-and-conquer sorts
- then heap-based and gap-based approaches
- finish with non-comparison and distribution-based sorts

---

## Summary

This Sorting module gives you:
- classic educational sorting algorithms
- practical baseline comparisons
- per-language demos
- cross-language benchmark tooling

It works as both:
- a learning project for understanding algorithm design
- and a benchmark playground for comparing implementations in C, Java, and Python
