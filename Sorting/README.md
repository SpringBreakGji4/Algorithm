# Sorting Algorithm Project

This repository is organized so each programming language can live in its own folder.

## Layout

- `C/` - C implementation
- `Java/` - Java implementation
- `Python/` - Python implementation
- `test_case/` - shared input datasets
- `compare_benchmarks.py` - cross-language benchmark comparison runner

## C project structure

Inside `C/`:

- `include/sorting.h` - common declarations
- `src/algorithms.c` - sorting algorithm implementations
- `src/utils.c` - shared helpers for time, file loading, and validation
- `src/main.c` - interactive demo runner, benchmark summary, and TSV export mode

## Java project structure

Inside `Java/`:

- `src/SortingDemo.java` - interactive demo, sorting implementations, benchmark summary, and TSV export mode
- `bin/` - compiled class files

## Python project structure

Inside `Python/`:

- `sorting_demo.py` - interactive demo, sorting implementations, benchmark summary, and TSV export mode

## Shared test cases

All language implementations use the shared files in:

- `test_case/`

## Build and run the C demo

```bash
make
make run
./bin/sorting_demo benchmark 5
./bin/sorting_demo benchmark-tsv 5
```

## Build and run the Java demo

```bash
cd Java
javac -d bin src/SortingDemo.java
java -cp bin SortingDemo all
java -cp bin SortingDemo benchmark 5
java -cp bin SortingDemo benchmark-tsv 5
```

## Run the Python demo

```bash
cd Python
python3 sorting_demo.py all
python3 sorting_demo.py benchmark 5
python3 sorting_demo.py benchmark-tsv 5
```

## Cross-language comparison by algorithm

Fast mode is the default. It skips the slow quadratic algorithms (`bubble`, `selection`, `insertion`) so the comparison finishes quickly.

```bash
python3 compare_benchmarks.py
python3 compare_benchmarks.py 3
```

Run every algorithm, including the slow ones:

```bash
python3 compare_benchmarks.py --full
python3 compare_benchmarks.py --full 1
```

Compare only the slow quadratic sorts:

```bash
python3 compare_benchmarks.py --slow-only
```

The comparison script:
- builds the C and Java demos
- runs C, Java, and Python benchmark export mode
- groups results by algorithm
- prints per-algorithm cross-language rankings
- prints a summary matrix of average runtime in microseconds

## Features available across implementations

- run one sorting algorithm
- run all algorithms
- benchmark summary mode
- machine-readable TSV export mode
- repeated runs for more stable timing
- complexity labels
- pass/fail validation
- shared test-case inputs
