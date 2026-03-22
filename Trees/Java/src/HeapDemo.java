import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.PriorityQueue;

public class HeapDemo {
    private static final Path DEFAULT_QUERY_FILE = Paths.get("..", "test_case", "heap_queries.txt");

    private static class MinHeap {
        private PriorityQueue<Integer> heap = new PriorityQueue<>();

        void push(int value) {
            heap.offer(value);
        }

        Integer pop() {
            return heap.poll();
        }

        Integer peek() {
            return heap.peek();
        }

        void heapify(List<Integer> values) {
            heap = new PriorityQueue<>(values);
        }

        int size() {
            return heap.size();
        }

        List<Integer> values() {
            return new ArrayList<>(heap);
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

        MinHeap heap = new MinHeap();
        System.out.println("Heap / Priority Queue Demo (Java)");
        System.out.println("=================================");

        for (String rawLine : Files.readAllLines(queryFile)) {
            String line = rawLine.trim();
            if (line.isEmpty() || line.startsWith("#")) {
                continue;
            }

            String[] parts = line.split("\\s+");
            String command = parts[0].toLowerCase();

            switch (command) {
                case "push" -> {
                    int value = Integer.parseInt(parts[1]);
                    heap.push(value);
                    System.out.println("push " + value + " -> OK");
                }
                case "pop" -> {
                    Integer value = heap.pop();
                    System.out.println("pop -> " + (value == null ? "(empty)" : value));
                }
                case "peek" -> {
                    Integer value = heap.peek();
                    System.out.println("peek -> " + (value == null ? "(empty)" : value));
                }
                case "size" -> System.out.println("size -> " + heap.size());
                case "print" -> System.out.println("heap -> " + formatValues(heap.values()));
                case "heapify" -> {
                    List<Integer> values = new ArrayList<>();
                    for (int i = 1; i < parts.length; i++) {
                        values.add(Integer.parseInt(parts[i]));
                    }
                    heap.heapify(values);
                    System.out.println("heapify -> " + formatValues(heap.values()));
                }
                default -> System.out.println("unknown command: " + line);
            }
        }
    }
}
