from __future__ import annotations

import sys
from dataclasses import dataclass, field
from pathlib import Path
from typing import Dict

TEST_CASE_FILE = Path("..") / "test_case" / "trie_queries.txt"


@dataclass
class TrieNode:
    children: Dict[str, "TrieNode"] = field(default_factory=dict)
    is_end_of_word: bool = False


class Trie:
    def __init__(self) -> None:
        self.root = TrieNode()

    def insert(self, word: str) -> None:
        node = self.root
        for char in word:
            node = node.children.setdefault(char, TrieNode())
        node.is_end_of_word = True

    def search(self, word: str) -> bool:
        node = self._find_node(word)
        return node is not None and node.is_end_of_word

    def starts_with(self, prefix: str) -> bool:
        return self._find_node(prefix) is not None

    def delete(self, word: str) -> bool:
        return self._delete(self.root, word, 0)

    def count_words_with_prefix(self, prefix: str) -> int:
        node = self._find_node(prefix)
        if node is None:
            return 0
        return self._count_words(node)

    def autocomplete(self, prefix: str) -> list[str]:
        node = self._find_node(prefix)
        if node is None:
            return []
        results: list[str] = []
        self._collect_words(node, prefix, results)
        return results

    def _find_node(self, text: str) -> TrieNode | None:
        node = self.root
        for char in text:
            if char not in node.children:
                return None
            node = node.children[char]
        return node

    def _delete(self, node: TrieNode, word: str, depth: int) -> bool:
        if depth == len(word):
            if not node.is_end_of_word:
                return False
            node.is_end_of_word = False
            return len(node.children) == 0

        char = word[depth]
        child = node.children.get(char)
        if child is None:
            return False

        should_delete_child = self._delete(child, word, depth + 1)
        if should_delete_child:
            del node.children[char]
            return not node.is_end_of_word and len(node.children) == 0
        return False

    def _count_words(self, node: TrieNode) -> int:
        total = 1 if node.is_end_of_word else 0
        for child in node.children.values():
            total += self._count_words(child)
        return total

    def _collect_words(self, node: TrieNode, prefix: str, results: list[str]) -> None:
        if node.is_end_of_word:
            results.append(prefix)
        for char in sorted(node.children.keys()):
            self._collect_words(node.children[char], prefix + char, results)


def run_queries(file_path: Path) -> int:
    if not file_path.is_file():
        print(f"Query file not found: {file_path}", file=sys.stderr)
        return 1

    trie = Trie()
    print("Trie / Prefix Tree Demo (Python)")
    print("================================")

    for raw_line in file_path.read_text().splitlines():
        line = raw_line.strip()
        if not line or line.startswith("#"):
            continue

        parts = line.split(maxsplit=1)
        command = parts[0].lower()
        argument = parts[1] if len(parts) > 1 else ""

        if command == "insert":
            trie.insert(argument)
            print(f"insert {argument} -> OK")
        elif command == "search":
            print(f"search {argument} -> {'FOUND' if trie.search(argument) else 'NOT FOUND'}")
        elif command == "prefix":
            print(f"prefix {argument} -> {'YES' if trie.starts_with(argument) else 'NO'}")
        elif command == "delete":
            existed = trie.search(argument)
            trie.delete(argument)
            print(f"delete {argument} -> {'DELETED' if existed else 'NOT FOUND'}")
        elif command == "autocomplete":
            matches = trie.autocomplete(argument)
            print(f"autocomplete {argument} -> {', '.join(matches) if matches else '(none)'}")
        elif command == "countprefix":
            print(f"countprefix {argument} -> {trie.count_words_with_prefix(argument)}")
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
