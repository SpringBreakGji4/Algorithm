from __future__ import annotations

import sys
from dataclasses import dataclass
from pathlib import Path
from typing import Optional

TEST_CASE_FILE = Path("..") / "test_case" / "avl_queries.txt"


@dataclass
class AVLNode:
    value: int
    left: Optional["AVLNode"] = None
    right: Optional["AVLNode"] = None
    height: int = 1


class AVLTree:
    def __init__(self) -> None:
        self.root: Optional[AVLNode] = None

    def insert(self, value: int) -> None:
        self.root = self._insert(self.root, value)

    def _insert(self, node: Optional[AVLNode], value: int) -> AVLNode:
        if node is None:
            return AVLNode(value)
        if value < node.value:
            node.left = self._insert(node.left, value)
        elif value > node.value:
            node.right = self._insert(node.right, value)
        else:
            return node

        self._update_height(node)
        return self._rebalance(node, value)

    def delete(self, value: int) -> None:
        self.root = self._delete(self.root, value)

    def _delete(self, node: Optional[AVLNode], value: int) -> Optional[AVLNode]:
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

        self._update_height(node)
        return self._rebalance_after_delete(node)

    def search(self, value: int) -> bool:
        node = self.root
        while node is not None:
            if value == node.value:
                return True
            node = node.left if value < node.value else node.right
        return False

    def inorder(self) -> list[int]:
        result: list[int] = []
        self._inorder(self.root, result)
        return result

    def _inorder(self, node: Optional[AVLNode], result: list[int]) -> None:
        if node is None:
            return
        self._inorder(node.left, result)
        result.append(node.value)
        self._inorder(node.right, result)

    def preorder(self) -> list[int]:
        result: list[int] = []
        self._preorder(self.root, result)
        return result

    def _preorder(self, node: Optional[AVLNode], result: list[int]) -> None:
        if node is None:
            return
        result.append(node.value)
        self._preorder(node.left, result)
        self._preorder(node.right, result)

    def height_value(self) -> int:
        return self._height(self.root) - 1

    def _height(self, node: Optional[AVLNode]) -> int:
        return node.height if node else 0

    def _balance(self, node: Optional[AVLNode]) -> int:
        return self._height(node.left) - self._height(node.right) if node else 0

    def _update_height(self, node: AVLNode) -> None:
        node.height = 1 + max(self._height(node.left), self._height(node.right))

    def _rotate_right(self, y: AVLNode) -> AVLNode:
        x = y.left
        t2 = x.right if x else None
        x.right = y
        y.left = t2
        self._update_height(y)
        self._update_height(x)
        return x

    def _rotate_left(self, x: AVLNode) -> AVLNode:
        y = x.right
        t2 = y.left if y else None
        y.left = x
        x.right = t2
        self._update_height(x)
        self._update_height(y)
        return y

    def _rebalance(self, node: AVLNode, inserted_value: int) -> AVLNode:
        balance = self._balance(node)

        if balance > 1 and inserted_value < node.left.value:
            return self._rotate_right(node)
        if balance < -1 and inserted_value > node.right.value:
            return self._rotate_left(node)
        if balance > 1 and inserted_value > node.left.value:
            node.left = self._rotate_left(node.left)
            return self._rotate_right(node)
        if balance < -1 and inserted_value < node.right.value:
            node.right = self._rotate_right(node.right)
            return self._rotate_left(node)
        return node

    def _rebalance_after_delete(self, node: AVLNode) -> AVLNode:
        balance = self._balance(node)

        if balance > 1 and self._balance(node.left) >= 0:
            return self._rotate_right(node)
        if balance > 1 and self._balance(node.left) < 0:
            node.left = self._rotate_left(node.left)
            return self._rotate_right(node)
        if balance < -1 and self._balance(node.right) <= 0:
            return self._rotate_left(node)
        if balance < -1 and self._balance(node.right) > 0:
            node.right = self._rotate_right(node.right)
            return self._rotate_left(node)
        return node

    def _min_node(self, node: AVLNode) -> AVLNode:
        current = node
        while current.left is not None:
            current = current.left
        return current


def format_values(values: list[int]) -> str:
    return " ".join(str(value) for value in values) if values else "(empty)"


def run_queries(file_path: Path) -> int:
    if not file_path.is_file():
        print(f"Query file not found: {file_path}", file=sys.stderr)
        return 1

    tree = AVLTree()
    print("AVL Tree Demo (Python)")
    print("======================")

    for raw_line in file_path.read_text().splitlines():
        line = raw_line.strip()
        if not line or line.startswith("#"):
            continue

        parts = line.split(maxsplit=1)
        command = parts[0].lower()
        argument = parts[1] if len(parts) > 1 else ""

        if command == "insert":
            tree.insert(int(argument))
            print(f"insert {argument} -> OK")
        elif command == "delete":
            existed = tree.search(int(argument))
            tree.delete(int(argument))
            print(f"delete {argument} -> {'DELETED' if existed else 'NOT FOUND'}")
        elif command == "search":
            print(f"search {argument} -> {'FOUND' if tree.search(int(argument)) else 'NOT FOUND'}")
        elif command == "inorder":
            print(f"inorder -> {format_values(tree.inorder())}")
        elif command == "preorder":
            print(f"preorder -> {format_values(tree.preorder())}")
        elif command == "height":
            print(f"height -> {tree.height_value()}")
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
