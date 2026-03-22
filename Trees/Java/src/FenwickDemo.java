import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Arrays;

public class FenwickDemo {
    private static final Path DEFAULT_QUERY_FILE = Paths.get("..", "test_case", "fenwick_queries.txt");

    private static class FenwickTree {
        private int[] tree = new int[1];
        private int[] values = new int[0];

        void build(int[] initialValues) {
            values = Arrays.copyOf(initialValues, initialValues.length);
            tree = new int[initialValues.length + 1];
            for (int i = 0; i < initialValues.length; i++) {
                add(i + 1, initialValues[i]);
            }
        }

        void update(int index, int delta) {
            values[index - 1] += delta;
            add(index, delta);
        }

        void setValue(int index, int value) {
            int current = values[index - 1];
            update(index, value - current);
        }

        int prefixSum(int index) {
            int sum = 0;
            while (index > 0) {
                sum += tree[index];
                index -= index & -index;
            }
            return sum;
        }

        int rangeSum(int left, int right) {
            return prefixSum(right) - prefixSum(left - 1);
        }

        int[] values() {
            return Arrays.copyOf(values, values.length);
        }

        private void add(int index, int delta) {
            while (index < tree.length) {
                tree[index] += delta;
                index += index & -index;
            }
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

        FenwickTree fenwick = new FenwickTree();
        System.out.println("Fenwick Tree Demo (Java)");
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
                    fenwick.build(values);
                    System.out.println("build -> " + formatValues(fenwick.values()));
                }
                case "print" -> System.out.println("values -> " + formatValues(fenwick.values()));
                case "prefix" -> {
                    int index = Integer.parseInt(parts[1]);
                    System.out.println("prefix " + index + " -> " + fenwick.prefixSum(index));
                }
                case "range" -> {
                    int left = Integer.parseInt(parts[1]);
                    int right = Integer.parseInt(parts[2]);
                    System.out.println("range " + left + " " + right + " -> " + fenwick.rangeSum(left, right));
                }
                case "update" -> {
                    int index = Integer.parseInt(parts[1]);
                    int delta = Integer.parseInt(parts[2]);
                    fenwick.update(index, delta);
                    System.out.println("update " + index + " " + delta + " -> OK");
                }
                case "set" -> {
                    int index = Integer.parseInt(parts[1]);
                    int value = Integer.parseInt(parts[2]);
                    fenwick.setValue(index, value);
                    System.out.println("set " + index + " " + value + " -> OK");
                }
                default -> System.out.println("unknown command: " + line);
            }
        }
    }
}
