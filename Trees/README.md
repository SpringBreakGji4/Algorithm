# Trees Module

This module contains tree-based data structures and related range-query structures implemented across multiple languages.

## Implemented structures

- Trie / Prefix Tree
- Binary Search Tree (BST)
- Heap / Priority Queue (min-heap)
- AVL Tree
- Fenwick Tree / Binary Indexed Tree
- Segment Tree

Implemented in:
- `C/`
- `Java/`
- `Python/`

Shared input files live in:
- `test_case/`

---

## Folder layout

```text
Trees/
├── C/
├── Java/
├── Python/
├── test_case/
└── README.md
```

### Current shared test cases

- `test_case/trie_queries.txt`
- `test_case/bst_queries.txt`
- `test_case/heap_queries.txt`
- `test_case/avl_queries.txt`
- `test_case/fenwick_queries.txt`
- `test_case/segment_tree_queries.txt`

---

## How each structure works

### 1. Trie / Prefix Tree

A Trie stores strings character by character in a tree.

#### ASCII view

```text
(root)
  |
  a
  |
  p
  |
  p*
  |
  l
  |
  e*
```

`*` marks the end of a complete word.

#### Core idea
- each node represents a character position in a word
- edges connect characters in sequence
- a flag marks whether a node completes a full word

For example, inserting `app` and `apple` shares the path:
- `a -> p -> p`
- then `apple` continues with `l -> e`

#### Why use it
- fast prefix lookup
- autocomplete
- dictionary-like word storage
- efficient `startsWith` checks

#### Operations implemented
- insert word
- search word
- prefix check
- delete word
- autocomplete
- count words with a prefix

#### Typical complexity
- insert: `O(L)`
- search: `O(L)`
- prefix query: `O(L)`
- autocomplete: depends on number of matching descendants

`L` = word length

---

### 2. Binary Search Tree (BST)

A BST stores values so that:
- every value in the left subtree is smaller than the node
- every value in the right subtree is larger than the node

#### ASCII view

```text
        50
       /  \
     30    70
    / \    / \
  20  40  60  80
```

#### Core idea
This ordering makes searching efficient when the tree is reasonably balanced.

Example:
- insert `50, 30, 70, 20, 40`
- values smaller than `50` go left
- values larger than `50` go right

#### Why use it
- ordered storage
- in-order traversal gives sorted output
- supports insert/search/delete naturally

#### Operations implemented
- insert
- search
- delete
- min
- max
- inorder traversal
- preorder traversal
- postorder traversal
- height

#### Typical complexity
- average insert/search/delete: `O(log n)`
- worst case: `O(n)` if the tree becomes skewed

---

### 3. Heap / Priority Queue

The current implementation uses a **min-heap**.

#### ASCII view

```text
        10
      /    \
    20      30
   /  \    /  \
 40   50  60   70

Array form: [10, 20, 30, 40, 50, 60, 70]
```

#### Core idea
A min-heap is a complete binary tree where:
- each parent is less than or equal to its children
- the minimum value is always at the root

It is usually stored in an array rather than explicit node pointers.

Array relationships:
- parent of `i` -> `(i - 1) / 2`
- left child -> `2i + 1`
- right child -> `2i + 2`

#### Why use it
- priority queues
- efficient minimum extraction
- scheduling / greedy algorithms
- Dijkstra / Prim support

#### Operations implemented
- push
- pop
- peek
- size
- print heap array
- heapify from an array

#### Typical complexity
- push: `O(log n)`
- pop: `O(log n)`
- peek: `O(1)`
- heapify: `O(n)`

---

### 4. AVL Tree

An AVL tree is a **self-balancing BST**.

#### ASCII view

```text
Before rebalancing:          After right rotation:

        30                           20
       /                            /  \
     20                           10    30
    /
  10
```

#### Core idea
After insertions and deletions, it checks the balance factor:
- `balance = height(left) - height(right)`

If the tree becomes too unbalanced, it performs rotations:
- left rotation
- right rotation
- left-right rotation
- right-left rotation

#### Why use it
- keeps BST operations efficient
- avoids the worst-case skewed-tree problem of a plain BST
- guarantees logarithmic height

#### Operations implemented
- insert
- delete
- search
- inorder traversal
- preorder traversal
- height

#### Typical complexity
- insert: `O(log n)`
- search: `O(log n)`
- delete: `O(log n)`

#### Rotations used
- LL case -> right rotation
- RR case -> left rotation
- LR case -> left rotation on child, then right rotation
- RL case -> right rotation on child, then left rotation

---

### 5. Fenwick Tree / Binary Indexed Tree

A Fenwick Tree is a compact structure for prefix sums.

#### ASCII view

```text
Index:  1   2   3   4   5   6   7   8
Value: a1  a2  a3  a4  a5  a6  a7  a8
Tree:  t1  t2  t3  t4  t5  t6  t7  t8

Covered ranges:
t1 -> [1]
t2 -> [1..2]
t3 -> [3]
t4 -> [1..4]
t5 -> [5]
t6 -> [5..6]
t7 -> [7]
t8 -> [1..8]
```

#### Core idea
It stores partial sums in a way that lets you:
- update one element quickly
- query prefix sums quickly

It uses bit tricks based on the least significant set bit.

Index movement pattern:
- move upward with `i += i & -i`
- move downward with `i -= i & -i`

#### Why use it
- dynamic prefix sums
- range sum queries when updates happen often
- simpler and lighter than a segment tree for some problems

#### Operations implemented
- build from array
- print current values
- prefix sum
- range sum
- update by delta
- set value directly

#### Typical complexity
- build: `O(n log n)` in this implementation
- update: `O(log n)`
- prefix sum: `O(log n)`
- range sum: `O(log n)`

---

### 6. Segment Tree

A Segment Tree stores information about ranges of an array.

#### ASCII view

```text
                 [0..7]
               /        \
           [0..3]      [4..7]
          /    \       /    \
      [0..1] [2..3] [4..5] [6..7]
      /  \    /  \   /  \    /  \
    [0][1] [2][3] [4][5] [6][7]
```

#### Core idea
Each node represents a segment of the array.
For range-sum queries:
- the root stores the sum of the full range
- children store sums of subranges
- queries descend only into segments that overlap the requested interval

#### Why use it
- efficient range queries
- efficient point updates
- adaptable to other operations besides sum:
  - minimum
  - maximum
  - gcd
  - lazy propagation variants later

#### Operations implemented
- build from array
- print current values
- range sum query
- update by delta
- set value directly

#### Typical complexity
- build: `O(n)`
- query: `O(log n)`
- update: `O(log n)`

---

## Language implementations

### C
Structure:
- `include/` for headers
- `src/` for implementations and demos
- `Makefile` for building demos

Current demos:
- `trie_demo`
- `bst_demo`
- `heap_demo`
- `avl_demo`
- `fenwick_demo`
- `segment_tree_demo`

Build:
```bash
cd C
make
```

Run examples:
```bash
make run-trie
make run-bst
make run-heap
make run-avl
make run-fenwick
make run-segment
```

### Java
Source files:
- `TrieDemo.java`
- `BSTDemo.java`
- `HeapDemo.java`
- `AVLDemo.java`
- `FenwickDemo.java`
- `SegmentTreeDemo.java`

Build:
```bash
cd Java
mkdir -p bin
javac -d bin src/*.java
```

Run examples:
```bash
java -cp bin TrieDemo
java -cp bin BSTDemo
java -cp bin HeapDemo
java -cp bin AVLDemo
java -cp bin FenwickDemo
java -cp bin SegmentTreeDemo
```

### Python
Current demos:
- `trie_demo.py`
- `bst_demo.py`
- `heap_demo.py`
- `avl_demo.py`
- `fenwick_demo.py`
- `segment_tree_demo.py`

Run examples:
```bash
cd Python
python3 trie_demo.py
python3 bst_demo.py
python3 heap_demo.py
python3 avl_demo.py
python3 fenwick_demo.py
python3 segment_tree_demo.py
```

---

## Suggested learning order

If someone is learning these structures from scratch, a good order is:

1. Binary Search Tree
2. Heap / Priority Queue
3. Trie / Prefix Tree
4. AVL Tree
5. Fenwick Tree
6. Segment Tree

Why:
- BST and heap build intuition for tree organization
- Trie shows non-numeric branching by characters
- AVL introduces balancing logic
- Fenwick and Segment Tree introduce range-query data structures

---

## Summary

This Trees module now includes both classic tree structures and practical range-query trees, with aligned demos in C, Java, and Python. It is a solid base for adding more advanced structures later, such as:
- Red-Black Tree
- B-Tree / B+ Tree
- Interval Tree
- Suffix Tree
- Lazy Segment Tree
