import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;

public class AVLDemo {
    private static final Path DEFAULT_QUERY_FILE = Paths.get("..", "test_case", "avl_queries.txt");

    private static class Node {
        int value;
        int height = 1;
        Node left;
        Node right;

        Node(int value) {
            this.value = value;
        }
    }

    private static class AVLTree {
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
            } else {
                return node;
            }

            updateHeight(node);
            return rebalanceInsert(node, value);
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
                Node successor = minNode(node.right);
                node.value = successor.value;
                node.right = delete(node.right, successor.value);
            }

            updateHeight(node);
            return rebalanceDelete(node);
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

        int heightValue() {
            return height(root) - 1;
        }

        private int height(Node node) {
            return node == null ? 0 : node.height;
        }

        private void updateHeight(Node node) {
            node.height = 1 + Math.max(height(node.left), height(node.right));
        }

        private int balance(Node node) {
            return node == null ? 0 : height(node.left) - height(node.right);
        }

        private Node rotateRight(Node y) {
            Node x = y.left;
            Node t2 = x.right;
            x.right = y;
            y.left = t2;
            updateHeight(y);
            updateHeight(x);
            return x;
        }

        private Node rotateLeft(Node x) {
            Node y = x.right;
            Node t2 = y.left;
            y.left = x;
            x.right = t2;
            updateHeight(x);
            updateHeight(y);
            return y;
        }

        private Node rebalanceInsert(Node node, int insertedValue) {
            int balance = balance(node);
            if (balance > 1 && insertedValue < node.left.value) {
                return rotateRight(node);
            }
            if (balance < -1 && insertedValue > node.right.value) {
                return rotateLeft(node);
            }
            if (balance > 1 && insertedValue > node.left.value) {
                node.left = rotateLeft(node.left);
                return rotateRight(node);
            }
            if (balance < -1 && insertedValue < node.right.value) {
                node.right = rotateRight(node.right);
                return rotateLeft(node);
            }
            return node;
        }

        private Node rebalanceDelete(Node node) {
            int balance = balance(node);
            if (balance > 1 && balance(node.left) >= 0) {
                return rotateRight(node);
            }
            if (balance > 1 && balance(node.left) < 0) {
                node.left = rotateLeft(node.left);
                return rotateRight(node);
            }
            if (balance < -1 && balance(node.right) <= 0) {
                return rotateLeft(node);
            }
            if (balance < -1 && balance(node.right) > 0) {
                node.right = rotateRight(node.right);
                return rotateLeft(node);
            }
            return node;
        }

        private Node minNode(Node node) {
            Node current = node;
            while (current.left != null) {
                current = current.left;
            }
            return current;
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

        AVLTree tree = new AVLTree();
        System.out.println("AVL Tree Demo (Java)");
        System.out.println("====================");

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
                    tree.insert(Integer.parseInt(argument));
                    System.out.println("insert " + argument + " -> OK");
                }
                case "delete" -> {
                    int value = Integer.parseInt(argument);
                    boolean existed = tree.search(value);
                    tree.delete(value);
                    System.out.println("delete " + argument + " -> " + (existed ? "DELETED" : "NOT FOUND"));
                }
                case "search" -> System.out.println("search " + argument + " -> " + (tree.search(Integer.parseInt(argument)) ? "FOUND" : "NOT FOUND"));
                case "inorder" -> System.out.println("inorder -> " + formatValues(tree.inorder()));
                case "preorder" -> System.out.println("preorder -> " + formatValues(tree.preorder()));
                case "height" -> System.out.println("height -> " + tree.heightValue());
                default -> System.out.println("unknown command: " + line);
            }
        }
    }
}
