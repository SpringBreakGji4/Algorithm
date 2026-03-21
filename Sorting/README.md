# Sorting Algorithm Project

This repository is organized so each programming language can live in its own folder.

## Layout

- `C/` - C implementation
- `test_case/` - shared input datasets
- future folders can be added later, for example:
  - `Java/`
  - `Python/`

## C project structure

Inside `C/`:

- `include/sorting.h` - common declarations
- `src/algorithms.c` - sorting algorithm implementations
- `src/utils.c` - shared helpers for time, file loading, and validation
- `src/main.c` - interactive demo runner and benchmark summary

## Build the C demo

```bash
make
```

## Run the C interactive demo

```bash
./bin/sorting_demo
```

## Run one C algorithm

```bash
./bin/sorting_demo quick
./bin/sorting_demo merge
./bin/sorting_demo radix
```

## Run one algorithm with repeated timing

```bash
./bin/sorting_demo quick 5
```

## Run all C algorithms with detailed output and summary

```bash
make run
```

or:

```bash
./bin/sorting_demo all
./bin/sorting_demo all 5
```

## Run benchmark summary only

```bash
./bin/sorting_demo benchmark
./bin/sorting_demo benchmark 5
```

The benchmark summary includes:
- speed ranking
- average runtime per testcase
- complexity labels
- pass/fail validation
- short notes for each algorithm
