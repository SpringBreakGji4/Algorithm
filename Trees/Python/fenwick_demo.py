from __future__ import annotations

import sys
from pathlib import Path

TEST_CASE_FILE = Path("..") / "test_case" / "fenwick_queries.txt"


class FenwickTree:
    def __init__(self, values: list[int] | None = None) -> None:
        self.values: list[int] = []
        self.tree: list[int] = [0]
        if values is not None:
            self.build(values)

    def build(self, values: list[int]) -> None:
        self.values = values[:]
        self.tree = [0] * (len(values) + 1)
        for index, value in enumerate(values, start=1):
            self._add(index, value)

    def update(self, index: int, delta: int) -> None:
        self.values[index - 1] += delta
        self._add(index, delta)

    def set_value(self, index: int, value: int) -> None:
        current = self.values[index - 1]
        self.update(index, value - current)

    def prefix_sum(self, index: int) -> int:
        result = 0
        while index > 0:
            result += self.tree[index]
            index -= index & -index
        return result

    def range_sum(self, left: int, right: int) -> int:
        return self.prefix_sum(right) - self.prefix_sum(left - 1)

    def _add(self, index: int, delta: int) -> None:
        while index < len(self.tree):
            self.tree[index] += delta
            index += index & -index


def format_values(values: list[int]) -> str:
    return " ".join(str(value) for value in values) if values else "(empty)"


def run_queries(file_path: Path) -> int:
    if not file_path.is_file():
        print(f"Query file not found: {file_path}", file=sys.stderr)
        return 1

    fenwick = FenwickTree()
    print("Fenwick Tree Demo (Python)")
    print("==========================")

    for raw_line in file_path.read_text().splitlines():
        line = raw_line.strip()
        if not line or line.startswith("#"):
            continue

        parts = line.split()
        command = parts[0].lower()

        if command == "build":
            values = [int(token) for token in parts[1:]]
            fenwick.build(values)
            print(f"build -> {format_values(fenwick.values)}")
        elif command == "print":
            print(f"values -> {format_values(fenwick.values)}")
        elif command == "prefix":
            index = int(parts[1])
            print(f"prefix {index} -> {fenwick.prefix_sum(index)}")
        elif command == "range":
            left = int(parts[1])
            right = int(parts[2])
            print(f"range {left} {right} -> {fenwick.range_sum(left, right)}")
        elif command == "update":
            index = int(parts[1])
            delta = int(parts[2])
            fenwick.update(index, delta)
            print(f"update {index} {delta} -> OK")
        elif command == "set":
            index = int(parts[1])
            value = int(parts[2])
            fenwick.set_value(index, value)
            print(f"set {index} {value} -> OK")
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
