import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;

public class BSTDemo {
    private static final Path DEFAULT_QUERY_FILE = Paths.get("..", "test_case", "bst_queries.txt");

    private static class Node {
        int value;
        Node left;
        Node right;

        Node(int value) {
            this.value = value;
        }
    }

    private static class BST {
        private Node root;

        void insert(int value) {
            root = insert(root, value);
        }

        private Node insert(Node node, int value) {
            if (node == null) {
                return new Node(value);
            }
            if (value < node.value) {
                node.left = insert(node.left, value);
            } else if (value > node.value) {
                node.right = insert(node.right, value);
            }
            return node;
        }

        boolean search(int value) {
            Node current = root;
            while (current != null) {
                if (value == current.value) {
                    return true;
                }
                current = value < current.value ? current.left : current.right;
            }
            return false;
        }

        void delete(int value) {
            root = delete(root, value);
        }

        private Node delete(Node node, int value) {
            if (node == null) {
                return null;
            }
            if (value < node.value) {
                node.left = delete(node.left, value);
            } else if (value > node.value) {
                node.right = delete(node.right, value);
            } else {
                if (node.left == null) {
                    return node.right;
                }
                if (node.right == null) {
                    return node.left;
                }
                Node successor = findMinNode(node.right);
                node.value = successor.value;
                node.right = delete(node.right, successor.value);
            }
            return node;
        }

        Integer minValue() {
            Node node = findMinNode(root);
            return node == null ? null : node.value;
        }

        private Node findMinNode(Node node) {
            Node current = node;
            while (current != null && current.left != null) {
                current = current.left;
            }
            return current;
        }

        Integer maxValue() {
            Node current = root;
            while (current != null && current.right != null) {
                current = current.right;
            }
            return current == null ? null : current.value;
        }

        List<Integer> inorder() {
            List<Integer> values = new ArrayList<>();
            inorder(root, values);
            return values;
        }

        private void inorder(Node node, List<Integer> values) {
            if (node == null) {
                return;
            }
            inorder(node.left, values);
            values.add(node.value);
            inorder(node.right, values);
        }

        List<Integer> preorder() {
            List<Integer> values = new ArrayList<>();
            preorder(root, values);
            return values;
        }

        private void preorder(Node node, List<Integer> values) {
            if (node == null) {
                return;
            }
            values.add(node.value);
            preorder(node.left, values);
            preorder(node.right, values);
        }

        List<Integer> postorder() {
            List<Integer> values = new ArrayList<>();
            postorder(root, values);
            return values;
        }

        private void postorder(Node node, List<Integer> values) {
            if (node == null) {
                return;
            }
            postorder(node.left, values);
            postorder(node.right, values);
            values.add(node.value);
        }

        int height() {
            return height(root);
        }

        private int height(Node node) {
            if (node == null) {
                return -1;
            }
            return 1 + Math.max(height(node.left), height(node.right));
        }
    }

    private static String formatValues(List<Integer> values) {
        if (values.isEmpty()) {
            return "(empty)";
        }
        StringBuilder builder = new StringBuilder();
        for (int i = 0; i < values.size(); i++) {
            if (i > 0) {
                builder.append(' ');
            }
            builder.append(values.get(i));
        }
        return builder.toString();
    }

    public static void main(String[] args) throws IOException {
        Path queryFile = args.length > 0 ? Paths.get(args[0]) : DEFAULT_QUERY_FILE;
        if (!Files.isRegularFile(queryFile)) {
            System.err.println("Query file not found: " + queryFile);
            return;
        }

        BST bst = new BST();
        System.out.println("Binary Search Tree Demo (Java)");
        System.out.println("==============================");

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
                    bst.insert(Integer.parseInt(argument));
                    System.out.println("insert " + argument + " -> OK");
                }
                case "search" -> System.out.println("search " + argument + " -> " + (bst.search(Integer.parseInt(argument)) ? "FOUND" : "NOT FOUND"));
                case "delete" -> {
                    int value = Integer.parseInt(argument);
                    boolean existed = bst.search(value);
                    bst.delete(value);
                    System.out.println("delete " + argument + " -> " + (existed ? "DELETED" : "NOT FOUND"));
                }
                case "min" -> {
                    Integer value = bst.minValue();
                    System.out.println("min -> " + (value == null ? "(empty)" : value));
                }
                case "max" -> {
                    Integer value = bst.maxValue();
                    System.out.println("max -> " + (value == null ? "(empty)" : value));
                }
                case "inorder" -> System.out.println("inorder -> " + formatValues(bst.inorder()));
                case "preorder" -> System.out.println("preorder -> " + formatValues(bst.preorder()));
                case "postorder" -> System.out.println("postorder -> " + formatValues(bst.postorder()));
                case "height" -> System.out.println("height -> " + bst.height());
                default -> System.out.println("unknown command: " + line);
            }
        }
    }
}
