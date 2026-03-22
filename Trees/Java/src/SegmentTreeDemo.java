import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Arrays;

public class SegmentTreeDemo {
    private static final Path DEFAULT_QUERY_FILE = Paths.get("..", "test_case", "segment_tree_queries.txt");

    private static class SegmentTree {
        private int[] tree = new int[0];
        private int[] values = new int[0];

        void build(int[] initialValues) {
            values = Arrays.copyOf(initialValues, initialValues.length);
            if (values.length == 0) {
                tree = new int[0];
                return;
            }
            tree = new int[values.length * 4];
            build(1, 0, values.length - 1);
        }

        private void build(int node, int left, int right) {
            if (left == right) {
                tree[node] = values[left];
                return;
            }
            int mid = (left + right) / 2;
            build(node * 2, left, mid);
            build(node * 2 + 1, mid + 1, right);
            tree[node] = tree[node * 2] + tree[node * 2 + 1];
        }

        int query(int left, int right) {
            if (values.length == 0) {
                return 0;
            }
            return query(1, 0, values.length - 1, left - 1, right - 1);
        }

        private int query(int node, int left, int right, int queryLeft, int queryRight) {
            if (queryRight < left || right < queryLeft) {
                return 0;
            }
            if (queryLeft <= left && right <= queryRight) {
                return tree[node];
            }
            int mid = (left + right) / 2;
            return query(node * 2, left, mid, queryLeft, queryRight)
                + query(node * 2 + 1, mid + 1, right, queryLeft, queryRight);
        }

        void update(int index, int delta) {
            values[index - 1] += delta;
            update(1, 0, values.length - 1, index - 1, values[index - 1]);
        }

        void setValue(int index, int value) {
            values[index - 1] = value;
            update(1, 0, values.length - 1, index - 1, value);
        }

        private void update(int node, int left, int right, int index, int value) {
            if (left == right) {
                tree[node] = value;
                return;
            }
            int mid = (left + right) / 2;
            if (index <= mid) {
                update(node * 2, left, mid, index, value);
            } else {
                update(node * 2 + 1, mid + 1, right, index, value);
            }
            tree[node] = tree[node * 2] + tree[node * 2 + 1];
        }

        int[] values() {
            return Arrays.copyOf(values, values.length);
        }
    }

    private static String formatValues(int[] values) {
        if (values.length == 0) {
            return "(empty)";
        }
        StringBuilder builder = new StringBuilder();
        for (int i = 0; i < values.length; i++) {
            if (i > 0) {
                builder.append(' ');
            }
            builder.append(values[i]);
        }
        return builder.toString();
    }

    public static void main(String[] args) throws IOException {
        Path queryFile = args.length > 0 ? Paths.get(args[0]) : DEFAULT_QUERY_FILE;
        if (!Files.isRegularFile(queryFile)) {
            System.err.println("Query file not found: " + queryFile);
            return;
        }

        SegmentTree segmentTree = new SegmentTree();
        System.out.println("Segment Tree Demo (Java)");
        System.out.println("========================");

        for (String rawLine : Files.readAllLines(queryFile)) {
            String line = rawLine.trim();
            if (line.isEmpty() || line.startsWith("#")) {
                continue;
            }

            String[] parts = line.split("\\s+");
            String command = parts[0].toLowerCase();

            switch (command) {
                case "build" -> {
                    int[] values = new int[parts.length - 1];
                    for (int i = 1; i < parts.length; i++) {
                        values[i - 1] = Integer.parseInt(parts[i]);
                    }
                    segmentTree.build(values);
                    System.out.println("build -> " + formatValues(segmentTree.values()));
                }
                case "print" -> System.out.println("values -> " + formatValues(segmentTree.values()));
                case "query" -> {
                    int left = Integer.parseInt(parts[1]);
                    int right = Integer.parseInt(parts[2]);
                    System.out.println("query " + left + " " + right + " -> " + segmentTree.query(left, right));
                }
                case "update" -> {
                    int index = Integer.parseInt(parts[1]);
                    int delta = Integer.parseInt(parts[2]);
                    segmentTree.update(index, delta);
                    System.out.println("update " + index + " " + delta + " -> OK");
                }
                case "set" -> {
                    int index = Integer.parseInt(parts[1]);
                    int value = Integer.parseInt(parts[2]);
                    segmentTree.setValue(index, value);
                    System.out.println("set " + index + " " + value + " -> OK");
                }
                default -> System.out.println("unknown command: " + line);
            }
        }
    }
}
