from __future__ import annotations

import sys
import time
from dataclasses import dataclass
from pathlib import Path
from typing import Callable

TEST_CASE_DIR = Path("..") / "test_case"
NUM_BUCKETS = 500
DEFAULT_REPEATS = 3


@dataclass(frozen=True)
class Algorithm:
    key: str
    name: str
    complexity: str
    notes: str
    sorter: Callable[[list[int]], None]


@dataclass
class BenchmarkResult:
    algorithm: Algorithm
    total_ns: int
    average_ns: int
    passed: bool


def load_test_cases() -> list[Path]:
    if not TEST_CASE_DIR.is_dir():
        return []
    return sorted((path for path in TEST_CASE_DIR.iterdir() if path.is_file()), key=lambda p: p.name)


def load_test_case(path: Path) -> list[int]:
    content = path.read_text().strip()
    if not content:
        return []
    return [int(value) for value in content.split()]


def is_sorted(array: list[int]) -> bool:
    return all(array[i] >= array[i - 1] for i in range(1, len(array)))


def bubble_sort(array: list[int]) -> None:
    for i in range(len(array) - 1):
        swapped = False
        for j in range(len(array) - i - 1):
            if array[j] > array[j + 1]:
                array[j], array[j + 1] = array[j + 1], array[j]
                swapped = True
        if not swapped:
            break


def selection_sort(array: list[int]) -> None:
    for i in range(len(array) - 1):
        min_index = i
        for j in range(i + 1, len(array)):
            if array[j] < array[min_index]:
                min_index = j
        array[i], array[min_index] = array[min_index], array[i]


def insertion_sort(array: list[int]) -> None:
    for i in range(1, len(array)):
        key = array[i]
        j = i - 1
        while j >= 0 and array[j] > key:
            array[j + 1] = array[j]
            j -= 1
        array[j + 1] = key


def shell_sort(array: list[int]) -> None:
    gap = len(array) // 2
    while gap > 0:
        for i in range(gap, len(array)):
            temp = array[i]
            j = i
            while j >= gap and array[j - gap] > temp:
                array[j] = array[j - gap]
                j -= gap
            array[j] = temp
        gap //= 2


def merge_sort(array: list[int]) -> None:
    if len(array) <= 1:
        return
    buffer = array.copy()
    _merge_sort_recursive(array, buffer, 0, len(array) - 1)


def _merge_sort_recursive(array: list[int], buffer: list[int], left: int, right: int) -> None:
    if left >= right:
        return
    mid = left + (right - left) // 2
    _merge_sort_recursive(array, buffer, left, mid)
    _merge_sort_recursive(array, buffer, mid + 1, right)
    _merge(array, buffer, left, mid, right)


def _merge(array: list[int], buffer: list[int], left: int, mid: int, right: int) -> None:
    i = left
    j = mid + 1
    k = left

    while i <= mid and j <= right:
        if array[i] <= array[j]:
            buffer[k] = array[i]
            i += 1
        else:
            buffer[k] = array[j]
            j += 1
        k += 1

    while i <= mid:
        buffer[k] = array[i]
        i += 1
        k += 1

    while j <= right:
        buffer[k] = array[j]
        j += 1
        k += 1

    array[left:right + 1] = buffer[left:right + 1]


def heap_sort(array: list[int]) -> None:
    n = len(array)
    for i in range(n // 2 - 1, -1, -1):
        _heapify(array, n, i)
    for i in range(n - 1, 0, -1):
        array[0], array[i] = array[i], array[0]
        _heapify(array, i, 0)


def _heapify(array: list[int], size: int, root: int) -> None:
    largest = root
    left = 2 * root + 1
    right = 2 * root + 2

    if left < size and array[left] > array[largest]:
        largest = left
    if right < size and array[right] > array[largest]:
        largest = right

    if largest != root:
        array[root], array[largest] = array[largest], array[root]
        _heapify(array, size, largest)


def quick_sort(array: list[int]) -> None:
    _quick_sort_recursive(array, 0, len(array) - 1)


def _quick_sort_recursive(array: list[int], left: int, right: int) -> None:
    if left >= right:
        return

    pivot = _median_of_three(array[left], array[left + (right - left) // 2], array[right])
    i = left
    j = right

    while i <= j:
        while array[i] < pivot:
            i += 1
        while array[j] > pivot:
            j -= 1
        if i <= j:
            array[i], array[j] = array[j], array[i]
            i += 1
            j -= 1

    if left < j:
        _quick_sort_recursive(array, left, j)
    if i < right:
        _quick_sort_recursive(array, i, right)


def _median_of_three(a: int, b: int, c: int) -> int:
    if (a <= b <= c) or (c <= b <= a):
        return b
    if (b <= a <= c) or (c <= a <= b):
        return a
    return c


def radix_sort(array: list[int]) -> None:
    if len(array) <= 1:
        return
    min_value = min(array)
    max_value = max(array)
    offset = -min_value if min_value < 0 else 0
    max_value += offset
    exp = 1
    while max_value // exp > 0:
        _counting_sort_by_digit(array, exp, offset)
        exp *= 10


def _counting_sort_by_digit(array: list[int], exp: int, offset: int) -> None:
    output = [0] * len(array)
    count = [0] * 10

    for value in array:
        count[((value + offset) // exp) % 10] += 1
    for i in range(1, 10):
        count[i] += count[i - 1]
    for value in reversed(array):
        digit = ((value + offset) // exp) % 10
        output[count[digit] - 1] = value
        count[digit] -= 1
    array[:] = output


def bucket_sort(array: list[int]) -> None:
    if len(array) <= 1:
        return

    min_value = min(array)
    max_value = max(array)
    value_range = max_value - min_value + 1
    if value_range <= 0:
        value_range = 1

    buckets: list[list[int]] = [[] for _ in range(NUM_BUCKETS)]
    for value in array:
        index = ((value - min_value) * (NUM_BUCKETS - 1)) // value_range
        index = max(0, min(NUM_BUCKETS - 1, index))
        buckets[index].append(value)

    out = 0
    for bucket in buckets:
        bucket.sort()
        for value in bucket:
            array[out] = value
            out += 1


def python_sort(array: list[int]) -> None:
    array.sort()


ALGORITHMS = [
    Algorithm("bubble", "Bubble sort", "O(n^2)", "stable, educational, very slow on large inputs", bubble_sort),
    Algorithm("selection", "Selection sort", "O(n^2)", "in-place, educational, usually slower than insertion", selection_sort),
    Algorithm("insertion", "Insertion sort", "O(n^2)", "good for small or nearly sorted arrays", insertion_sort),
    Algorithm("shell", "Shell sort", "~O(n log n) to O(n^2)", "gap-based improvement over insertion sort", shell_sort),
    Algorithm("merge", "Merge sort", "O(n log n)", "stable, consistent, needs extra memory", merge_sort),
    Algorithm("heap", "Heap sort", "O(n log n)", "in-place, consistent worst-case performance", heap_sort),
    Algorithm("quick", "Quick sort", "avg O(n log n)", "usually fast in practice, worst-case O(n^2)", quick_sort),
    Algorithm("radix", "Radix sort", "O(d(n+b))", "non-comparison sort, data-dependent performance", radix_sort),
    Algorithm("bucket", "Bucket sort", "avg O(n+k)", "best when values distribute well", bucket_sort),
    Algorithm("python", "Python list.sort", "impl-dependent", "Python standard library baseline", python_sort),
]


def run_algorithm(algorithm: Algorithm, test_cases: list[Path], repeats: int, verbose: bool) -> BenchmarkResult:
    total_ns = 0
    passed = True

    if verbose:
        print(f"\n---{algorithm.name}---")
        print(f"Complexity: {algorithm.complexity}")
        print(f"Notes: {algorithm.notes}")

    for index, test_case in enumerate(test_cases, start=1):
        input_values = load_test_case(test_case)
        case_total = 0
        case_passed = True

        for _ in range(repeats):
            copy = input_values.copy()
            start = time.perf_counter_ns()
            algorithm.sorter(copy)
            elapsed = time.perf_counter_ns() - start
            total_ns += elapsed
            case_total += elapsed
            if not is_sorted(copy):
                passed = False
                case_passed = False

        if verbose:
            avg_usec = (case_total // repeats) // 1000
            suffix = "" if repeats == 1 else "s"
            print(f"Test case {index} ... {'Pass!' if case_passed else 'Fail ...'} (avg {avg_usec} usec over {repeats} run{suffix})")

    average_ns = total_ns // (len(test_cases) * repeats)
    if verbose:
        print(f"Total time: {total_ns // 1000} usec")
        print(f"Average per testcase: {average_ns // 1000} usec")

    return BenchmarkResult(algorithm, total_ns, average_ns, passed)


def print_summary(results: list[BenchmarkResult], repeats: int) -> None:
    results.sort(key=lambda result: (result.average_ns, result.algorithm.name))
    print("\nBenchmark Summary")
    print("=================")
    print(f"Runs per testcase: {repeats}\n")
    print(f"{'Rank':<4} {'Algorithm':<20} {'Complexity':<20} {'Avg(usec)':<14} {'Status':<8} Notes")
    for index, result in enumerate(results, start=1):
        print(
            f"{index:<4} {result.algorithm.name:<20} {result.algorithm.complexity:<20} "
            f"{result.average_ns // 1000:<14} {'PASS' if result.passed else 'FAIL':<8} {result.algorithm.notes}"
        )
    print()


def print_csv(results: list[BenchmarkResult], repeats: int, language: str) -> None:
    print("language\tkey\tname\tcomplexity\tnotes\taverage_us\ttotal_us\tpassed\trepeats")
    for result in results:
        print(
            f"{language}\t{result.algorithm.key}\t{result.algorithm.name}\t{result.algorithm.complexity}\t"
            f"{result.algorithm.notes}\t{result.average_ns // 1000}\t{result.total_ns // 1000}\t"
            f"{'true' if result.passed else 'false'}\t{repeats}"
        )


def run_all_algorithms(test_cases: list[Path], repeats: int, verbose: bool, csv_only: bool = False) -> None:
    results = [run_algorithm(algorithm, test_cases, repeats, verbose) for algorithm in ALGORITHMS]
    if csv_only:
        print_csv(results, repeats, "Python")
    else:
        print_summary(results, repeats)


def find_algorithm(key: str) -> Algorithm | None:
    for algorithm in ALGORITHMS:
        if algorithm.key == key:
            return algorithm
    return None


def print_menu() -> None:
    print("Sorting Algorithm Demo (Python)")
    print("===============================")
    for index, algorithm in enumerate(ALGORITHMS, start=1):
        print(f"{index:2d}. {algorithm.name} ({algorithm.key})")
    print(" a. Run all algorithms with benchmark summary")
    print(" b. Benchmark all algorithms (summary only)")
    print(" q. Quit")


def main() -> int:
    test_cases = load_test_cases()
    if not test_cases:
        print(f"No test cases found in {TEST_CASE_DIR}", file=sys.stderr)
        return 1

    repeats = DEFAULT_REPEATS
    if len(sys.argv) > 2:
        try:
            repeats = int(sys.argv[2])
            if repeats <= 0:
                raise ValueError
        except ValueError:
            print(f"Invalid repeat count: {sys.argv[2]}", file=sys.stderr)
            return 1

    if len(sys.argv) > 1:
        mode = sys.argv[1].lower()
        if mode == "all":
            run_all_algorithms(test_cases, repeats, True)
            return 0
        if mode == "benchmark":
            run_all_algorithms(test_cases, repeats, False)
            return 0
        if mode == "benchmark-tsv":
            run_all_algorithms(test_cases, repeats, False, True)
            return 0
        algorithm = find_algorithm(mode)
        if algorithm is not None:
            run_algorithm(algorithm, test_cases, repeats, True)
            return 0
        print(f"Unknown algorithm: {sys.argv[1]}", file=sys.stderr)
        return 1

    while True:
        print_menu()
        choice = input("\nSelect an option: ").strip().lower()
        if choice == "q":
            break
        if choice == "a":
            run_all_algorithms(test_cases, repeats, True)
            continue
        if choice == "b":
            run_all_algorithms(test_cases, repeats, False)
            continue
        if choice.isdigit():
            numeric = int(choice)
            if 1 <= numeric <= len(ALGORITHMS):
                run_algorithm(ALGORITHMS[numeric - 1], test_cases, repeats, True)
                continue
        algorithm = find_algorithm(choice)
        if algorithm is not None:
            run_algorithm(algorithm, test_cases, repeats, True)
            continue
        print("Invalid selection. Try again.\n")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
temExit(main())
