import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.List;
import java.util.Locale;
import java.util.Scanner;
import java.util.function.Consumer;
import java.util.stream.Collectors;

public class SortingDemo {
    private static final Path TEST_CASE_DIR = Paths.get("..", "test_case");
    private static final int NUM_BUCKETS = 500;
    private static final int DEFAULT_REPEATS = 3;

    private record Algorithm(String key, String name, String complexity, String notes, Consumer<int[]> sorter) {}
    private record BenchmarkResult(Algorithm algorithm, long totalNanos, long averageNanos, boolean passed) {}

    private static final List<Algorithm> ALGORITHMS = List.of(
        new Algorithm("bubble", "Bubble sort", "O(n^2)", "stable, educational, very slow on large inputs", SortingDemo::bubbleSort),
        new Algorithm("selection", "Selection sort", "O(n^2)", "in-place, educational, usually slower than insertion", SortingDemo::selectionSort),
        new Algorithm("insertion", "Insertion sort", "O(n^2)", "good for small or nearly sorted arrays", SortingDemo::insertionSort),
        new Algorithm("shell", "Shell sort", "~O(n log n) to O(n^2)", "gap-based improvement over insertion sort", SortingDemo::shellSort),
        new Algorithm("merge", "Merge sort", "O(n log n)", "stable, consistent, needs extra memory", SortingDemo::mergeSort),
        new Algorithm("heap", "Heap sort", "O(n log n)", "in-place, consistent worst-case performance", SortingDemo::heapSort),
        new Algorithm("quick", "Quick sort", "avg O(n log n)", "usually fast in practice, worst-case O(n^2)", SortingDemo::quickSort),
        new Algorithm("radix", "Radix sort", "O(d(n+b))", "non-comparison sort, data-dependent performance", SortingDemo::radixSort),
        new Algorithm("bucket", "Bucket sort", "avg O(n+k)", "best when values distribute well", SortingDemo::bucketSort),
        new Algorithm("arrays", "Arrays.sort", "impl-dependent", "Java standard library baseline", Arrays::sort)
    );

    public static void main(String[] args) throws IOException {
        List<Path> testCases = collectTestCases();
        if (testCases.isEmpty()) {
            System.err.println("No test cases found in " + TEST_CASE_DIR.normalize());
            return;
        }

        int repeats = DEFAULT_REPEATS;
        if (args.length > 1) {
            repeats = parseRepeats(args[1]);
        }

        if (args.length > 0) {
            String mode = args[0].toLowerCase(Locale.ROOT);
            if (mode.equals("all")) {
                runAllAlgorithms(testCases, repeats, true);
                return;
            }
            if (mode.equals("benchmark")) {
                runAllAlgorithms(testCases, repeats, false);
                return;
            }

            Algorithm algorithm = findAlgorithm(mode);
            if (algorithm != null) {
                runAlgorithm(algorithm, testCases, repeats, true);
                return;
            }

            System.err.println("Unknown algorithm: " + args[0]);
            return;
        }

        runInteractive(testCases, repeats);
    }

    private static void runInteractive(List<Path> testCases, int repeats) throws IOException {
        Scanner scanner = new Scanner(System.in);

        while (true) {
            printMenu();
            System.out.print("\nSelect an option: ");
            if (!scanner.hasNextLine()) {
                break;
            }

            String choice = scanner.nextLine().trim().toLowerCase(Locale.ROOT);
            if (choice.equals("q")) {
                break;
            }
            if (choice.equals("a")) {
                runAllAlgorithms(testCases, repeats, true);
                continue;
            }
            if (choice.equals("b")) {
                runAllAlgorithms(testCases, repeats, false);
                continue;
            }

            try {
                int numeric = Integer.parseInt(choice);
                if (numeric >= 1 && numeric <= ALGORITHMS.size()) {
                    runAlgorithm(ALGORITHMS.get(numeric - 1), testCases, repeats, true);
                    continue;
                }
            } catch (NumberFormatException ignored) {
            }

            Algorithm algorithm = findAlgorithm(choice);
            if (algorithm != null) {
                runAlgorithm(algorithm, testCases, repeats, true);
                continue;
            }

            System.out.println("Invalid selection. Try again.\n");
        }
    }

    private static void printMenu() {
        System.out.println("Sorting Algorithm Demo (Java)");
        System.out.println("=============================");
        for (int i = 0; i < ALGORITHMS.size(); i++) {
            Algorithm algorithm = ALGORITHMS.get(i);
            System.out.printf("%2d. %s (%s)%n", i + 1, algorithm.name(), algorithm.key());
        }
        System.out.println(" a. Run all algorithms with benchmark summary");
        System.out.println(" b. Benchmark all algorithms (summary only)");
        System.out.println(" q. Quit");
    }

    private static List<Path> collectTestCases() throws IOException {
        if (!Files.isDirectory(TEST_CASE_DIR)) {
            return List.of();
        }

        try (var stream = Files.list(TEST_CASE_DIR)) {
            return stream
                .filter(Files::isRegularFile)
                .sorted(Comparator.comparing(path -> path.getFileName().toString()))
                .collect(Collectors.toList());
        }
    }

    private static int[] loadTestCase(Path path) throws IOException {
        String content = Files.readString(path);
        if (content.isBlank()) {
            return new int[0];
        }
        String[] parts = content.trim().split("\\s+");
        int[] values = new int[parts.length];
        for (int i = 0; i < parts.length; i++) {
            values[i] = Integer.parseInt(parts[i]);
        }
        return values;
    }

    private static Algorithm findAlgorithm(String key) {
        for (Algorithm algorithm : ALGORITHMS) {
            if (algorithm.key().equalsIgnoreCase(key)) {
                return algorithm;
            }
        }
        return null;
    }

    private static int parseRepeats(String value) {
        try {
            int repeats = Integer.parseInt(value);
            if (repeats <= 0) {
                throw new IllegalArgumentException("Repeat count must be positive.");
            }
            return repeats;
        } catch (NumberFormatException ex) {
            throw new IllegalArgumentException("Invalid repeat count: " + value, ex);
        }
    }

    private static void runAllAlgorithms(List<Path> testCases, int repeats, boolean verbose) throws IOException {
        List<BenchmarkResult> results = new ArrayList<>();
        for (Algorithm algorithm : ALGORITHMS) {
            results.add(runAlgorithm(algorithm, testCases, repeats, verbose));
        }
        printSummary(results, repeats);
    }

    private static BenchmarkResult runAlgorithm(Algorithm algorithm, List<Path> testCases, int repeats, boolean verbose) throws IOException {
        long totalNanos = 0L;
        boolean passed = true;

        if (verbose) {
            System.out.printf("%n---%s---%n", algorithm.name());
            System.out.println("Complexity: " + algorithm.complexity());
            System.out.println("Notes: " + algorithm.notes());
        }

        for (int i = 0; i < testCases.size(); i++) {
            int[] input = loadTestCase(testCases.get(i));
            long caseTotal = 0L;
            boolean casePassed = true;

            for (int repeat = 0; repeat < repeats; repeat++) {
                int[] copy = Arrays.copyOf(input, input.length);
                long start = System.nanoTime();
                algorithm.sorter().accept(copy);
                long elapsed = System.nanoTime() - start;
                caseTotal += elapsed;
                totalNanos += elapsed;

                if (!isSorted(copy)) {
                    casePassed = false;
                    passed = false;
                }
            }

            if (verbose) {
                long avgMicros = nanosToMicros(caseTotal / repeats);
                System.out.printf("Test case %d ... %s (avg %d usec over %d run%s)%n",
                    i + 1,
                    casePassed ? "Pass!" : "Fail ...",
                    avgMicros,
                    repeats,
                    repeats == 1 ? "" : "s");
            }
        }

        long averageNanos = totalNanos / (testCases.size() * repeats);
        if (verbose) {
            System.out.println("Total time: " + nanosToMicros(totalNanos) + " usec");
            System.out.println("Average per testcase: " + nanosToMicros(averageNanos) + " usec");
        }

        return new BenchmarkResult(algorithm, totalNanos, averageNanos, passed);
    }

    private static void printSummary(List<BenchmarkResult> results, int repeats) {
        results.sort(Comparator.comparingLong(BenchmarkResult::averageNanos)
            .thenComparing(result -> result.algorithm().name()));

        System.out.println("\nBenchmark Summary");
        System.out.println("=================");
        System.out.println("Runs per testcase: " + repeats + "\n");
        System.out.printf("%-4s %-20s %-20s %-14s %-8s %s%n",
            "Rank", "Algorithm", "Complexity", "Avg(usec)", "Status", "Notes");

        for (int i = 0; i < results.size(); i++) {
            BenchmarkResult result = results.get(i);
            System.out.printf("%-4d %-20s %-20s %-14d %-8s %s%n",
                i + 1,
                result.algorithm().name(),
                result.algorithm().complexity(),
                nanosToMicros(result.averageNanos()),
                result.passed() ? "PASS" : "FAIL",
                result.algorithm().notes());
        }
        System.out.println();
    }

    private static long nanosToMicros(long nanos) {
        return nanos / 1_000L;
    }

    private static boolean isSorted(int[] array) {
        for (int i = 1; i < array.length; i++) {
            if (array[i] < array[i - 1]) {
                return false;
            }
        }
        return true;
    }

    private static void bubbleSort(int[] array) {
        for (int i = 0; i < array.length - 1; i++) {
            boolean swapped = false;
            for (int j = 0; j < array.length - i - 1; j++) {
                if (array[j] > array[j + 1]) {
                    swap(array, j, j + 1);
                    swapped = true;
                }
            }
            if (!swapped) {
                break;
            }
        }
    }

    private static void selectionSort(int[] array) {
        for (int i = 0; i < array.length - 1; i++) {
            int minIndex = i;
            for (int j = i + 1; j < array.length; j++) {
                if (array[j] < array[minIndex]) {
                    minIndex = j;
                }
            }
            swap(array, i, minIndex);
        }
    }

    private static void insertionSort(int[] array) {
        for (int i = 1; i < array.length; i++) {
            int key = array[i];
            int j = i - 1;
            while (j >= 0 && array[j] > key) {
                array[j + 1] = array[j];
                j--;
            }
            array[j + 1] = key;
        }
    }

    private static void shellSort(int[] array) {
        for (int gap = array.length / 2; gap > 0; gap /= 2) {
            for (int i = gap; i < array.length; i++) {
                int temp = array[i];
                int j = i;
                while (j >= gap && array[j - gap] > temp) {
                    array[j] = array[j - gap];
                    j -= gap;
                }
                array[j] = temp;
            }
        }
    }

    private static void mergeSort(int[] array) {
        if (array.length <= 1) {
            return;
        }
        int[] buffer = new int[array.length];
        mergeSortRecursive(array, buffer, 0, array.length - 1);
    }

    private static void mergeSortRecursive(int[] array, int[] buffer, int left, int right) {
        if (left >= right) {
            return;
        }
        int mid = left + (right - left) / 2;
        mergeSortRecursive(array, buffer, left, mid);
        mergeSortRecursive(array, buffer, mid + 1, right);
        merge(array, buffer, left, mid, right);
    }

    private static void merge(int[] array, int[] buffer, int left, int mid, int right) {
        int i = left;
        int j = mid + 1;
        int k = left;

        while (i <= mid && j <= right) {
            if (array[i] <= array[j]) {
                buffer[k++] = array[i++];
            } else {
                buffer[k++] = array[j++];
            }
        }
        while (i <= mid) {
            buffer[k++] = array[i++];
        }
        while (j <= right) {
            buffer[k++] = array[j++];
        }
        System.arraycopy(buffer, left, array, left, right - left + 1);
    }

    private static void heapSort(int[] array) {
        for (int i = array.length / 2 - 1; i >= 0; i--) {
            heapify(array, array.length, i);
        }
        for (int i = array.length - 1; i > 0; i--) {
            swap(array, 0, i);
            heapify(array, i, 0);
        }
    }

    private static void heapify(int[] array, int size, int root) {
        int largest = root;
        int left = 2 * root + 1;
        int right = 2 * root + 2;

        if (left < size && array[left] > array[largest]) {
            largest = left;
        }
        if (right < size && array[right] > array[largest]) {
            largest = right;
        }
        if (largest != root) {
            swap(array, root, largest);
            heapify(array, size, largest);
        }
    }

    private static void quickSort(int[] array) {
        quickSortRecursive(array, 0, array.length - 1);
    }

    private static void quickSortRecursive(int[] array, int left, int right) {
        if (left >= right) {
            return;
        }

        int pivot = medianOfThree(array[left], array[left + (right - left) / 2], array[right]);
        int i = left;
        int j = right;

        while (i <= j) {
            while (array[i] < pivot) {
                i++;
            }
            while (array[j] > pivot) {
                j--;
            }
            if (i <= j) {
                swap(array, i, j);
                i++;
                j--;
            }
        }

        if (left < j) {
            quickSortRecursive(array, left, j);
        }
        if (i < right) {
            quickSortRecursive(array, i, right);
        }
    }

    private static int medianOfThree(int a, int b, int c) {
        if ((a <= b && b <= c) || (c <= b && b <= a)) {
            return b;
        }
        if ((b <= a && a <= c) || (c <= a && a <= b)) {
            return a;
        }
        return c;
    }

    private static void radixSort(int[] array) {
        if (array.length <= 1) {
            return;
        }
        int min = array[0];
        int max = array[0];
        for (int value : array) {
            min = Math.min(min, value);
            max = Math.max(max, value);
        }
        int offset = min < 0 ? -min : 0;
        max += offset;
        for (int exp = 1; max / exp > 0; exp *= 10) {
            countingSortByDigit(array, exp, offset);
        }
    }

    private static void countingSortByDigit(int[] array, int exp, int offset) {
        int[] output = new int[array.length];
        int[] count = new int[10];

        for (int value : array) {
            count[((value + offset) / exp) % 10]++;
        }
        for (int i = 1; i < 10; i++) {
            count[i] += count[i - 1];
        }
        for (int i = array.length - 1; i >= 0; i--) {
            int digit = ((array[i] + offset) / exp) % 10;
            output[count[digit] - 1] = array[i];
            count[digit]--;
        }
        System.arraycopy(output, 0, array, 0, array.length);
    }

    private static void bucketSort(int[] array) {
        if (array.length <= 1) {
            return;
        }
        int min = array[0];
        int max = array[0];
        for (int value : array) {
            min = Math.min(min, value);
            max = Math.max(max, value);
        }

        int range = max - min + 1;
        if (range <= 0) {
            range = 1;
        }

        List<List<Integer>> buckets = new ArrayList<>(NUM_BUCKETS);
        for (int i = 0; i < NUM_BUCKETS; i++) {
            buckets.add(new ArrayList<>());
        }

        for (int value : array) {
            int index = (int) (((long) (value - min) * (NUM_BUCKETS - 1)) / range);
            index = Math.max(0, Math.min(NUM_BUCKETS - 1, index));
            buckets.get(index).add(value);
        }

        int out = 0;
        for (List<Integer> bucket : buckets) {
            bucket.sort(Integer::compareTo);
            for (int value : bucket) {
                array[out++] = value;
            }
        }
    }

    private static void swap(int[] array, int i, int j) {
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}
