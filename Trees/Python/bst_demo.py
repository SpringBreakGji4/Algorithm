from __future__ import annotations

import sys
from dataclasses import dataclass
from pathlib import Path
from typing import Optional

TEST_CASE_FILE = Path("..") / "test_case" / "bst_queries.txt"


@dataclass
class BSTNode:
    value: int
    left: Optional["BSTNode"] = None
    right: Optional["BSTNode"] = None


class BST:
    def __init__(self) -> None:
        self.root: Optional[BSTNode] = None

    def insert(self, value: int) -> None:
        self.root = self._insert(self.root, value)

    def _insert(self, node: Optional[BSTNode], value: int) -> BSTNode:
        if node is None:
            return BSTNode(value)
        if value < node.value:
            node.left = self._insert(node.left, value)
        elif value > node.value:
            node.right = self._insert(node.right, value)
        return node

    def search(self, value: int) -> bool:
        node = self.root
        while node is not None:
            if value == node.value:
                return True
            node = node.left if value < node.value else node.right
        return False

    def delete(self, value: int) -> None:
        self.root = self._delete(self.root, value)

    def _delete(self, node: Optional[BSTNode], value: int) -> Optional[BSTNode]:
        if node is None:
            return None
        if value < node.value:
            node.left = self._delete(node.left, value)
        elif value > node.value:
            node.right = self._delete(node.right, value)
        else:
            if node.left is None:
                return node.right
            if node.right is None:
                return node.left
            successor = self._min_node(node.right)
            node.value = successor.value
            node.right = self._delete(node.right, successor.value)
        return node

    def min_value(self) -> Optional[int]:
        node = self._min_node(self.root)
        return node.value if node else None

    def _min_node(self, node: Optional[BSTNode]) -> Optional[BSTNode]:
        current = node
        while current and current.left:
            current = current.left
        return current

    def max_value(self) -> Optional[int]:
        current = self.root
        while current and current.right:
            current = current.right
        return current.value if current else None

    def inorder(self) -> list[int]:
        result: list[int] = []
        self._inorder(self.root, result)
        return result

    def _inorder(self, node: Optional[BSTNode], result: list[int]) -> None:
        if node is None:
            return
        self._inorder(node.left, result)
        result.append(node.value)
        self._inorder(node.right, result)

    def preorder(self) -> list[int]:
        result: list[int] = []
        self._preorder(self.root, result)
        return result

    def _preorder(self, node: Optional[BSTNode], result: list[int]) -> None:
        if node is None:
            return
        result.append(node.value)
        self._preorder(node.left, result)
        self._preorder(node.right, result)

    def postorder(self) -> list[int]:
        result: list[int] = []
        self._postorder(self.root, result)
        return result

    def _postorder(self, node: Optional[BSTNode], result: list[int]) -> None:
        if node is None:
            return
        self._postorder(node.left, result)
        self._postorder(node.right, result)
        result.append(node.value)

    def height(self) -> int:
        return self._height(self.root)

    def _height(self, node: Optional[BSTNode]) -> int:
        if node is None:
            return -1
        return 1 + max(self._height(node.left), self._height(node.right))


def format_values(values: list[int]) -> str:
    return " ".join(str(value) for value in values) if values else "(empty)"


def run_queries(file_path: Path) -> int:
    if not file_path.is_file():
        print(f"Query file not found: {file_path}", file=sys.stderr)
        return 1

    bst = BST()
    print("Binary Search Tree Demo (Python)")
    print("================================")

    for raw_line in file_path.read_text().splitlines():
        line = raw_line.strip()
        if not line or line.startswith("#"):
            continue

        parts = line.split(maxsplit=1)
        command = parts[0].lower()
        argument = parts[1] if len(parts) > 1 else ""

        if command == "insert":
            bst.insert(int(argument))
            print(f"insert {argument} -> OK")
        elif command == "search":
            print(f"search {argument} -> {'FOUND' if bst.search(int(argument)) else 'NOT FOUND'}")
        elif command == "delete":
            existed = bst.search(int(argument))
            bst.delete(int(argument))
            print(f"delete {argument} -> {'DELETED' if existed else 'NOT FOUND'}")
        elif command == "min":
            value = bst.min_value()
            print(f"min -> {value if value is not None else '(empty)'}")
        elif command == "max":
            value = bst.max_value()
            print(f"max -> {value if value is not None else '(empty)'}")
        elif command == "inorder":
            print(f"inorder -> {format_values(bst.inorder())}")
        elif command == "preorder":
            print(f"preorder -> {format_values(bst.preorder())}")
        elif command == "postorder":
            print(f"postorder -> {format_values(bst.postorder())}")
        elif command == "height":
            print(f"height -> {bst.height()}")
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
