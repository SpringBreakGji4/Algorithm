import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

public class TrieDemo {
    private static final Path DEFAULT_QUERY_FILE = Paths.get("..", "test_case", "trie_queries.txt");

    private static class TrieNode {
        Map<Character, TrieNode> children = new TreeMap<>();
        boolean isEndOfWord = false;
    }

    private static class Trie {
        private final TrieNode root = new TrieNode();

        void insert(String word) {
            TrieNode current = root;
            for (char rawChar : word.toCharArray()) {
                char ch = Character.toLowerCase(rawChar);
                if (!Character.isLetter(ch)) {
                    continue;
                }
                current = current.children.computeIfAbsent(ch, key -> new TrieNode());
            }
            current.isEndOfWord = true;
        }

        boolean search(String word) {
            TrieNode node = findNode(word);
            return node != null && node.isEndOfWord;
        }

        boolean startsWith(String prefix) {
            return findNode(prefix) != null;
        }

        boolean delete(String word) {
            return deleteRecursive(root, normalize(word), 0).deleted;
        }

        int countPrefix(String prefix) {
            TrieNode node = findNode(prefix);
            if (node == null) {
                return 0;
            }
            return countWords(node);
        }

        List<String> autocomplete(String prefix) {
            TrieNode node = findNode(prefix);
            if (node == null) {
                return List.of();
            }
            List<String> results = new ArrayList<>();
            collectWords(node, normalize(prefix), results);
            return results;
        }

        private TrieNode findNode(String text) {
            TrieNode current = root;
            for (char ch : normalize(text).toCharArray()) {
                current = current.children.get(ch);
                if (current == null) {
                    return null;
                }
            }
            return current;
        }

        private String normalize(String text) {
            StringBuilder builder = new StringBuilder();
            for (char rawChar : text.toCharArray()) {
                char ch = Character.toLowerCase(rawChar);
                if (Character.isLetter(ch)) {
                    builder.append(ch);
                }
            }
            return builder.toString();
        }

        private DeleteResult deleteRecursive(TrieNode node, String word, int depth) {
            if (depth == word.length()) {
                if (!node.isEndOfWord) {
                    return new DeleteResult(false, false);
                }
                node.isEndOfWord = false;
                return new DeleteResult(true, node.children.isEmpty());
            }

            char ch = word.charAt(depth);
            TrieNode child = node.children.get(ch);
            if (child == null) {
                return new DeleteResult(false, false);
            }

            DeleteResult result = deleteRecursive(child, word, depth + 1);
            if (result.shouldPruneChild) {
                node.children.remove(ch);
            }

            boolean shouldPrune = !node.isEndOfWord && node.children.isEmpty();
            return new DeleteResult(result.deleted, shouldPrune);
        }

        private int countWords(TrieNode node) {
            int total = node.isEndOfWord ? 1 : 0;
            for (TrieNode child : node.children.values()) {
                total += countWords(child);
            }
            return total;
        }

        private void collectWords(TrieNode node, String prefix, List<String> results) {
            if (node.isEndOfWord) {
                results.add(prefix);
            }
            for (Map.Entry<Character, TrieNode> entry : node.children.entrySet()) {
                collectWords(entry.getValue(), prefix + entry.getKey(), results);
            }
        }

        private record DeleteResult(boolean deleted, boolean shouldPruneChild) {}
    }

    public static void main(String[] args) throws IOException {
        Path queryFile = args.length > 0 ? Paths.get(args[0]) : DEFAULT_QUERY_FILE;
        if (!Files.isRegularFile(queryFile)) {
            System.err.println("Query file not found: " + queryFile);
            return;
        }

        Trie trie = new Trie();
        System.out.println("Trie / Prefix Tree Demo (Java)");
        System.out.println("===============================");

        for (String rawLine : Files.readAllLines(queryFile)) {
            String line = rawLine.trim();
            if (line.isEmpty() || line.startsWith("#")) {
                continue;
            }

            String[] parts = line.split("\\s+", 2);
            String command = parts[0].toLowerCase();
            String argument = parts.length > 1 ? parts[1] : "";

            switch (command) {
                case "insert" -> {
                    trie.insert(argument);
                    System.out.println("insert " + argument + " -> OK");
                }
                case "search" -> System.out.println("search " + argument + " -> " + (trie.search(argument) ? "FOUND" : "NOT FOUND"));
                case "prefix" -> System.out.println("prefix " + argument + " -> " + (trie.startsWith(argument) ? "YES" : "NO"));
                case "delete" -> System.out.println("delete " + argument + " -> " + (trie.delete(argument) ? "DELETED" : "NOT FOUND"));
                case "autocomplete" -> {
                    List<String> matches = trie.autocomplete(argument);
                    System.out.println("autocomplete " + argument + " -> " + (matches.isEmpty() ? "(none)" : String.join(", ", matches)));
                }
                case "countprefix" -> System.out.println("countprefix " + argument + " -> " + trie.countPrefix(argument));
                default -> System.out.println("unknown command: " + line);
            }
        }
    }
}
