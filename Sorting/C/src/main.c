#include "sorting.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEST_CASE_DIR "../test_case"
#define MAX_PATH_LEN 512
#define DEFAULT_REPEATS 3

typedef void (*sort_fn)(int *, int);

typedef struct {
    const char *key;
    const char *name;
    const char *complexity;
    const char *notes;
    sort_fn fn;
} algorithm_entry;

typedef struct {
    const algorithm_entry *algorithm;
    long long total_us;
    long long average_us;
    int passed;
} benchmark_result;

static algorithm_entry algorithms[] = {
    {"bubble", "Bubble sort", "O(n^2)", "stable, educational, very slow on large inputs", bubble_sort},
    {"selection", "Selection sort", "O(n^2)", "in-place, educational, usually slower than insertion", selection_sort},
    {"insertion", "Insertion sort", "O(n^2)", "good for small or nearly sorted arrays", insertion_sort},
    {"shell", "Shell sort", "~O(n log n) to O(n^2)", "gap-based improvement over insertion sort", shell_sort},
    {"merge", "Merge sort", "O(n log n)", "stable, consistent, needs extra memory", merge_sort},
    {"heap", "Heap sort", "O(n log n)", "in-place, consistent worst-case performance", heap_sort},
    {"quick", "Quick sort", "avg O(n log n)", "usually fast in practice, worst-case O(n^2)", quick_sort},
    {"radix", "Radix sort", "O(d(n+b))", "non-comparison sort, data-dependent performance", radix_sort},
    {"bucket", "Bucket sort", "avg O(n+k)", "best when values distribute well", bucket_sort},
    {"qsort", "Built-in qsort", "impl-dependent", "C library sort for baseline comparison", libc_qsort_wrapper}
};

static int compare_names(const void *a, const void *b){
    const char *left = *(const char * const *)a;
    const char *right = *(const char * const *)b;
    return strcmp(left, right);
}

static int compare_benchmarks(const void *a, const void *b){
    const benchmark_result *left = (const benchmark_result *)a;
    const benchmark_result *right = (const benchmark_result *)b;

    if(left->average_us < right->average_us){
        return -1;
    }
    if(left->average_us > right->average_us){
        return 1;
    }
    return strcmp(left->algorithm->name, right->algorithm->name);
}

static int collect_test_cases(char paths[][MAX_PATH_LEN], int max_cases){
    DIR *dir = opendir(TEST_CASE_DIR);
    struct dirent *entry;
    char *names[256];
    int count = 0;

    if(dir == NULL){
        fprintf(stderr, "Error: failed to open %s\n", TEST_CASE_DIR);
        return -1;
    }

    while((entry = readdir(dir)) != NULL && count < max_cases){
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
            continue;
        }
        names[count] = strdup(entry->d_name);
        if(names[count] == NULL){
            closedir(dir);
            return -1;
        }
        count++;
    }
    closedir(dir);

    qsort(names, (size_t)count, sizeof(char *), compare_names);

    for(int i = 0; i < count; i++){
        snprintf(paths[i], MAX_PATH_LEN, "%s/%s", TEST_CASE_DIR, names[i]);
        free(names[i]);
    }

    return count;
}

static long long run_single_case(const algorithm_entry *algorithm, const int *input, int size, int *passed){
    int *array = (int *)malloc(sizeof(int) * size);
    if(array == NULL){
        *passed = 0;
        return -1;
    }

    memcpy(array, input, sizeof(int) * size);
    long long start = current_timestamp_us();
    algorithm->fn(array, size);
    long long elapsed = current_timestamp_us() - start;

    *passed = check_array_sorted(array, size);
    free(array);
    return elapsed;
}

static benchmark_result run_algorithm(const algorithm_entry *algorithm, char test_cases[][MAX_PATH_LEN], int case_count, int repeats, int verbose){
    int input[MAX_INPUT_SIZE];
    benchmark_result result;
    result.algorithm = algorithm;
    result.total_us = 0;
    result.average_us = 0;
    result.passed = 1;

    if(verbose){
        printf("\n---%s---\n", algorithm->name);
        printf("Complexity: %s\n", algorithm->complexity);
        printf("Notes: %s\n", algorithm->notes);
    }

    for(int i = 0; i < case_count; i++){
        int size = load_test_case(test_cases[i], input, MAX_INPUT_SIZE);
        if(size < 0){
            result.passed = 0;
            if(verbose){
                printf("Test case %d ... Fail to open file\n", i + 1);
            }
            continue;
        }

        long long case_total = 0;
        int case_passed = 1;

        for(int repeat = 0; repeat < repeats; repeat++){
            int passed = 0;
            long long elapsed = run_single_case(algorithm, input, size, &passed);
            if(elapsed < 0 || !passed){
                case_passed = 0;
                result.passed = 0;
            }
            if(elapsed >= 0){
                case_total += elapsed;
                result.total_us += elapsed;
            }
        }

        if(verbose){
            long long case_average = repeats > 0 ? case_total / repeats : 0;
            printf("Test case %d ... %s (avg %lld usec over %d run%s)\n",
                   i + 1,
                   case_passed ? "Pass!" : "Fail ...",
                   case_average,
                   repeats,
                   repeats == 1 ? "" : "s");
        }
    }

    result.average_us = (case_count > 0 && repeats > 0) ? result.total_us / (case_count * repeats) : 0;

    if(verbose){
        printf("Total time: %lld usec\n", result.total_us);
        printf("Average per testcase: %lld usec\n", result.average_us);
    }

    return result;
}

static void print_summary(benchmark_result *results, int count, int repeats){
    qsort(results, (size_t)count, sizeof(benchmark_result), compare_benchmarks);

    printf("\nBenchmark Summary\n");
    printf("=================\n");
    printf("Runs per testcase: %d\n\n", repeats);
    printf("%-4s %-20s %-18s %-14s %-8s %s\n", "Rank", "Algorithm", "Complexity", "Avg(usec)", "Status", "Notes");

    for(int i = 0; i < count; i++){
        printf("%-4d %-20s %-18s %-14lld %-8s %s\n",
               i + 1,
               results[i].algorithm->name,
               results[i].algorithm->complexity,
               results[i].average_us,
               results[i].passed ? "PASS" : "FAIL",
               results[i].algorithm->notes);
    }
    printf("\n");
}

static void print_tsv(benchmark_result *results, int count, const char *language, int repeats){
    printf("language\tkey\tname\tcomplexity\tnotes\taverage_us\ttotal_us\tpassed\trepeats\n");
    for(int i = 0; i < count; i++){
        printf("%s\t%s\t%s\t%s\t%s\t%lld\t%lld\t%s\t%d\n",
               language,
               results[i].algorithm->key,
               results[i].algorithm->name,
               results[i].algorithm->complexity,
               results[i].algorithm->notes,
               results[i].average_us,
               results[i].total_us,
               results[i].passed ? "true" : "false",
               repeats);
    }
}

static void print_menu(void){
    int count = (int)(sizeof(algorithms) / sizeof(algorithms[0]));

    printf("Sorting Algorithm Demo (C)\n");
    printf("==========================\n");
    for(int i = 0; i < count; i++){
        printf("%2d. %s (%s)\n", i + 1, algorithms[i].name, algorithms[i].key);
    }
    printf(" a. Run all algorithms with benchmark summary\n");
    printf(" b. Benchmark all algorithms (summary only)\n");
    printf(" q. Quit\n");
}

static int find_algorithm_by_key(const char *key){
    int count = (int)(sizeof(algorithms) / sizeof(algorithms[0]));

    for(int i = 0; i < count; i++){
        if(strcmp(key, algorithms[i].key) == 0){
            return i;
        }
    }

    return -1;
}

static int parse_repeats(const char *value){
    char *endptr = NULL;
    long parsed = strtol(value, &endptr, 10);
    if(endptr == value || *endptr != '\0' || parsed <= 0){
        return -1;
    }
    return (int)parsed;
}

static void collect_all_results(char test_cases[][MAX_PATH_LEN], int case_count, int repeats, int verbose, benchmark_result *results){
    int algorithm_count = (int)(sizeof(algorithms) / sizeof(algorithms[0]));

    for(int i = 0; i < algorithm_count; i++){
        results[i] = run_algorithm(&algorithms[i], test_cases, case_count, repeats, verbose);
    }
}

int main(int argc, char **argv){
    char test_cases[256][MAX_PATH_LEN];
    int case_count = collect_test_cases(test_cases, 256);
    int algorithm_count = (int)(sizeof(algorithms) / sizeof(algorithms[0]));
    int repeats = DEFAULT_REPEATS;

    if(case_count <= 0){
        return 1;
    }

    if(argc > 2){
        repeats = parse_repeats(argv[2]);
        if(repeats <= 0){
            fprintf(stderr, "Invalid repeat count: %s\n", argv[2]);
            return 1;
        }
    }

    if(argc > 1){
        if(strcmp(argv[1], "all") == 0){
            benchmark_result results[sizeof(algorithms) / sizeof(algorithms[0])];
            collect_all_results(test_cases, case_count, repeats, 1, results);
            print_summary(results, algorithm_count, repeats);
            return 0;
        }
        if(strcmp(argv[1], "benchmark") == 0){
            benchmark_result results[sizeof(algorithms) / sizeof(algorithms[0])];
            collect_all_results(test_cases, case_count, repeats, 0, results);
            print_summary(results, algorithm_count, repeats);
            return 0;
        }
        if(strcmp(argv[1], "benchmark-tsv") == 0){
            benchmark_result results[sizeof(algorithms) / sizeof(algorithms[0])];
            collect_all_results(test_cases, case_count, repeats, 0, results);
            print_tsv(results, algorithm_count, "C", repeats);
            return 0;
        }

        int index = find_algorithm_by_key(argv[1]);
        if(index >= 0){
            run_algorithm(&algorithms[index], test_cases, case_count, repeats, 1);
            return 0;
        }

        fprintf(stderr, "Unknown algorithm: %s\n", argv[1]);
        return 1;
    }

    for(;;){
        char choice[32];
        print_menu();
        printf("\nSelect an option: ");

        if(fgets(choice, sizeof(choice), stdin) == NULL){
            break;
        }

        choice[strcspn(choice, "\n")] = '\0';

        if(strcmp(choice, "q") == 0){
            break;
        }
        if(strcmp(choice, "a") == 0){
            benchmark_result results[sizeof(algorithms) / sizeof(algorithms[0])];
            collect_all_results(test_cases, case_count, repeats, 1, results);
            print_summary(results, algorithm_count, repeats);
            continue;
        }
        if(strcmp(choice, "b") == 0){
            benchmark_result results[sizeof(algorithms) / sizeof(algorithms[0])];
            collect_all_results(test_cases, case_count, repeats, 0, results);
            print_summary(results, algorithm_count, repeats);
            continue;
        }

        char *endptr = NULL;
        long numeric_choice = strtol(choice, &endptr, 10);
        if(endptr != choice && *endptr == '\0' && numeric_choice >= 1 && numeric_choice <= algorithm_count){
            run_algorithm(&algorithms[numeric_choice - 1], test_cases, case_count, repeats, 1);
            continue;
        }

        int index = find_algorithm_by_key(choice);
        if(index >= 0){
            run_algorithm(&algorithms[index], test_cases, case_count, repeats, 1);
            continue;
        }

        printf("Invalid selection. Try again.\n\n");
    }

    return 0;
}
