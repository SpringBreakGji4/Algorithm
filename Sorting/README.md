# Sorting Algorithm Project

This repository is organized so each programming language can live in its own folder.

## Layout

- `C/` - C implementation
- `Java/` - Java implementation
- `Python/` - Python implementation
- `test_case/` - shared input datasets

## C project structure

Inside `C/`:

- `include/sorting.h` - common declarations
- `src/algorithms.c` - sorting algorithm implementations
- `src/utils.c` - shared helpers for time, file loading, and validation
- `src/main.c` - interactive demo runner and benchmark summary

## Java project structure

Inside `Java/`:

- `src/SortingDemo.java` - interactive demo, sorting implementations, and benchmark summary
- `bin/` - compiled class files

## Python project structure

Inside `Python/`:

- `sorting_demo.py` - interactive demo, sorting implementations, and benchmark summary

## Shared test cases

All language implementations use the shared files in:

- `test_case/`

## Build and run the C demo

```bash
make
make run
./bin/sorting_demo benchmark 5
```

## Build and run the Java demo

```bash
cd Java
javac -d bin src/SortingDemo.java
java -cp bin SortingDemo all
java -cp bin SortingDemo benchmark 5
```

## Run the Python demo

```bash
cd Python
python3 sorting_demo.py all
python3 sorting_demo.py benchmark 5
```

## Features available across implementations

- run one sorting algorithm
- run all algorithms
- benchmark summary mode
- repeated runs for more stable timing
- complexity labels
- pass/fail validation
- shared test-case inputs
