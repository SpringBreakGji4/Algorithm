from __future__ import annotations

import sys
from pathlib import Path

TEST_CASE_FILE = Path("..") / "test_case" / "segment_tree_queries.txt"


class SegmentTree:
    def __init__(self) -> None:
        self.values: list[int] = []
        self.tree: list[int] = []

    def build(self, values: list[int]) -> None:
        self.values = values[:]
        if not values:
            self.tree = []
            return
        self.tree = [0] * (4 * len(values))
        self._build(1, 0, len(values) - 1)

    def _build(self, node: int, left: int, right: int) -> None:
        if left == right:
            self.tree[node] = self.values[left]
            return
        mid = (left + right) // 2
        self._build(node * 2, left, mid)
        self._build(node * 2 + 1, mid + 1, right)
        self.tree[node] = self.tree[node * 2] + self.tree[node * 2 + 1]

    def query(self, query_left: int, query_right: int) -> int:
        if not self.values:
            return 0
        return self._query(1, 0, len(self.values) - 1, query_left - 1, query_right - 1)

    def _query(self, node: int, left: int, right: int, query_left: int, query_right: int) -> int:
        if query_right < left or right < query_left:
            return 0
        if query_left <= left and right <= query_right:
            return self.tree[node]
        mid = (left + right) // 2
        return self._query(node * 2, left, mid, query_left, query_right) + self._query(node * 2 + 1, mid + 1, right, query_left, query_right)

    def update(self, index: int, delta: int) -> None:
        self.values[index - 1] += delta
        self._update(1, 0, len(self.values) - 1, index - 1, self.values[index - 1])

    def set_value(self, index: int, value: int) -> None:
        self.values[index - 1] = value
        self._update(1, 0, len(self.values) - 1, index - 1, value)

    def _update(self, node: int, left: int, right: int, index: int, value: int) -> None:
        if left == right:
            self.tree[node] = value
            return
        mid = (left + right) // 2
        if index <= mid:
            self._update(node * 2, left, mid, index, value)
        else:
            self._update(node * 2 + 1, mid + 1, right, index, value)
        self.tree[node] = self.tree[node * 2] + self.tree[node * 2 + 1]


def format_values(values: list[int]) -> str:
    return " ".join(str(value) for value in values) if values else "(empty)"


def run_queries(file_path: Path) -> int:
    if not file_path.is_file():
        print(f"Query file not found: {file_path}", file=sys.stderr)
        return 1

    segment_tree = SegmentTree()
    print("Segment Tree Demo (Python)")
    print("==========================")

    for raw_line in file_path.read_text().splitlines():
        line = raw_line.strip()
        if not line or line.startswith("#"):
            continue

        parts = line.split()
        command = parts[0].lower()

        if command == "build":
            values = [int(token) for token in parts[1:]]
            segment_tree.build(values)
            print(f"build -> {format_values(segment_tree.values)}")
        elif command == "print":
            print(f"values -> {format_values(segment_tree.values)}")
        elif command == "query":
            left = int(parts[1])
            right = int(parts[2])
            print(f"query {left} {right} -> {segment_tree.query(left, right)}")
        elif command == "update":
            index = int(parts[1])
            delta = int(parts[2])
            segment_tree.update(index, delta)
            print(f"update {index} {delta} -> OK")
        elif command == "set":
            index = int(parts[1])
            value = int(parts[2])
            segment_tree.set_value(index, value)
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
