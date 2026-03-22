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
└── README.md
```

---

## How each sorting algorithm works

### 1. Bubble Sort

Bubble sort repeatedly compares adjacent elements and swaps them if they are out of order.

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
