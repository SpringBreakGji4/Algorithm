from __future__ import annotations

import heapq
import sys
from pathlib import Path

TEST_CASE_FILE = Path("..") / "test_case" / "heap_queries.txt"


class MinHeap:
    def __init__(self) -> None:
        self.data: list[int] = []

    def push(self, value: int) -> None:
        heapq.heappush(self.data, value)

    def pop(self) -> int | None:
        if not self.data:
            return None
        return heapq.heappop(self.data)

    def peek(self) -> int | None:
        if not self.data:
            return None
        return self.data[0]

    def heapify(self, values: list[int]) -> None:
        self.data = values[:]
        heapq.heapify(self.data)

    def size(self) -> int:
        return len(self.data)

    def values(self) -> list[int]:
        return self.data[:]


def format_values(values: list[int]) -> str:
    return " ".join(str(value) for value in values) if values else "(empty)"


def run_queries(file_path: Path) -> int:
    if not file_path.is_file():
        print(f"Query file not found: {file_path}", file=sys.stderr)
        return 1

    heap = MinHeap()
    print("Heap / Priority Queue Demo (Python)")
    print("===================================")

    for raw_line in file_path.read_text().splitlines():
        line = raw_line.strip()
        if not line or line.startswith("#"):
            continue

        parts = line.split()
        command = parts[0].lower()

        if command == "push":
            value = int(parts[1])
            heap.push(value)
            print(f"push {value} -> OK")
        elif command == "pop":
            value = heap.pop()
            print(f"pop -> {value if value is not None else '(empty)'}")
        elif command == "peek":
            value = heap.peek()
            print(f"peek -> {value if value is not None else '(empty)'}")
        elif command == "size":
            print(f"size -> {heap.size()}")
        elif command == "print":
            print(f"heap -> {format_values(heap.values())}")
        elif command == "heapify":
            values = [int(token) for token in parts[1:]]
            heap.heapify(values)
            print(f"heapify -> {format_values(heap.values())}")
        else:
            print(f"unknown command: {line}")

    return 0


def main() -> int:
    query_file = TEST_CASE_FILE
    if len(sys.argv) > 1:
        query_file = Path(sys.argv[1])
    return run_queries(query_file)


if __name__ == "__main__":
    raise SystemExit(main())
