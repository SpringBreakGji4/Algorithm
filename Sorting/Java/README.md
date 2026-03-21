# Java Sorting Demo

This folder contains the Java implementation of the sorting algorithm demo and benchmark.

## File layout

- `src/SortingDemo.java` - interactive demo, algorithm implementations, and benchmark summary
- `bin/` - compiled class files after running `javac`

## Build

```bash
cd Java
javac -d bin src/SortingDemo.java
```

## Run interactive mode

```bash
cd Java
java -cp bin SortingDemo
```

## Run all algorithms with detailed output and summary

```bash
cd Java
java -cp bin SortingDemo all
java -cp bin SortingDemo all 5
```

## Run benchmark summary only

```bash
cd Java
java -cp bin SortingDemo benchmark
java -cp bin SortingDemo benchmark 5
```

## Run one algorithm

```bash
cd Java
java -cp bin SortingDemo quick
java -cp bin SortingDemo merge 5
```

The Java version reads shared test cases from `../test_case/` so it stays aligned with the C project.
