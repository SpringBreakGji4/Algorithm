# Python Sorting Demo

This folder contains the Python implementation of the sorting algorithm demo and benchmark.

## File layout

- `sorting_demo.py` - interactive demo, algorithm implementations, and benchmark summary

## Run interactive mode

```bash
cd Python
python3 sorting_demo.py
```

## Run all algorithms with detailed output and summary

```bash
cd Python
python3 sorting_demo.py all
python3 sorting_demo.py all 5
```

## Run benchmark summary only

```bash
cd Python
python3 sorting_demo.py benchmark
python3 sorting_demo.py benchmark 5
```

## Run one algorithm

```bash
cd Python
python3 sorting_demo.py quick
python3 sorting_demo.py merge 5
```

The Python version reads shared test cases from `../test_case/` so it stays aligned with the C and Java projects.
