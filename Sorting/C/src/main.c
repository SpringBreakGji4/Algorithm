#include "sorting.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEST_CASE_DIR "../test_case"
#define MAX_PATH_LEN 512

typedef void (*sort_fn)(int *, int);

typedef struct {
    const char *key;
    const char *name;
    sort_fn fn;
} algorithm_entry;

static algorithm_entry algorithms[] = {
    {"bubble", "Bubble sort", bubble_sort},
    {"selection", "Selection sort", selection_sort},
    {"insertion", "Insertion sort", insertion_sort},
    {"shell", "Shell sort", shell_sort},
    {"merge", "Merge sort", merge_sort},
    {"heap", "Heap sort", heap_sort},
    {"quick", "Quick sort", quick_sort},
    {"radix", "Radix sort", radix_sort},
    {"bucket", "Bucket sort", bucket_sort},
    {"qsort", "Built-in qsort", libc_qsort_wrapper}
};

static int compare_names(const void *a, const void *b){
    const char *left = *(const char * const *)a;
    const char *right = *(const char * const *)b;
    return strcmp(left, right);
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

static void run_algorithm(const algorithm_entry *algorithm, char test_cases[][MAX_PATH_LEN], int case_count){
    int input[MAX_INPUT_SIZE];

    printf("\n---%s---\n", algorithm->name);
    long long start = current_timestamp_us();

    for(int i = 0; i < case_count; i++){
        int size = load_test_case(test_cases[i], input, MAX_INPUT_SIZE);
        if(size < 0){
            printf("Test case %d ... Fail to open file\n", i + 1);
            continue;
        }

        int *array = (int *)malloc(sizeof(int) * size);
        if(array == NULL){
            printf("Test case %d ... Memory allocation failed\n", i + 1);
            continue;
        }

        memcpy(array, input, sizeof(int) * size);
        algorithm->fn(array, size);

        printf("Test case %d ... %s\n", i + 1, check_array_sorted(array, size) ? "Pass!" : "Fail ...");
        free(array);
    }

    long long end = current_timestamp_us();
    printf("time: %lld usec\n", end - start);
}

static void print_menu(void){
    int count = (int)(sizeof(algorithms) / sizeof(algorithms[0]));

    printf("Sorting Algorithm Demo (C)\n");
    printf("==========================\n");
    for(int i = 0; i < count; i++){
        printf("%2d. %s (%s)\n", i + 1, algorithms[i].name, algorithms[i].key);
    }
    printf(" a. Run all algorithms\n");
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

int main(int argc, char **argv){
    char test_cases[256][MAX_PATH_LEN];
    int case_count = collect_test_cases(test_cases, 256);
    int algorithm_count = (int)(sizeof(algorithms) / sizeof(algorithms[0]));

    if(case_count <= 0){
        return 1;
    }

    if(argc > 1){
        if(strcmp(argv[1], "all") == 0){
            for(int i = 0; i < algorithm_count; i++){
                run_algorithm(&algorithms[i], test_cases, case_count);
            }
            return 0;
        }

        int index = find_algorithm_by_key(argv[1]);
        if(index >= 0){
            run_algorithm(&algorithms[index], test_cases, case_count);
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
            for(int i = 0; i < algorithm_count; i++){
                run_algorithm(&algorithms[i], test_cases, case_count);
            }
            continue;
        }

        char *endptr = NULL;
        long numeric_choice = strtol(choice, &endptr, 10);
        if(endptr != choice && *endptr == '\0' && numeric_choice >= 1 && numeric_choice <= algorithm_count){
            run_algorithm(&algorithms[numeric_choice - 1], test_cases, case_count);
            continue;
        }

        int index = find_algorithm_by_key(choice);
        if(index >= 0){
            run_algorithm(&algorithms[index], test_cases, case_count);
            continue;
        }

        printf("Invalid selection. Try again.\n\n");
    }

    return 0;
}
