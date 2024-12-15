#include <stdlib.h>
#include <stdio.h>
#include <time.h>

enum ERRORS{
    OK,
    MEMORY_ERROR,
};

void rand_arr(int array[], int size);

int close_number(int number, int size, int array[]);

int numcmp(const void *number1, const void *number2);

void print_array(int array[], int size);

int main() {
    srand(time(NULL));

    int size_a, size_b;
    size_a = 3;
    size_b = 5;

    int *first_array = (int *) malloc(size_a * sizeof(int));
    int *second_array = (int *) malloc(size_b * sizeof(int));

    if (first_array == NULL || second_array == NULL) {
        printf("ERROR: MEMORY_ERROR.\n");
        free(first_array);
        free(second_array);
        return MEMORY_ERROR;
    }

    rand_arr(first_array, size_a);
    rand_arr(second_array, size_b);

    qsort(second_array, size_b, sizeof(int), numcmp);

    int *last_array = (int *) malloc(size_a * sizeof(int));
    if (last_array == NULL) {
        printf("ERROR: MEMORY_ERROR.\n");
        free(first_array);
        free(second_array);
        return MEMORY_ERROR;
    }

    printf("ARRAY A: \n");
    print_array(first_array, size_a);
    printf("ARRAY B (SORTED): \n");
    print_array(second_array, size_b);

    for (int i = 0; i < size_a; ++i) {
        last_array[i] = first_array[i] + close_number(first_array[i], size_b, second_array);
    }

    printf("ARRAY C: \n");
    print_array(last_array, size_a);

    free(first_array);
    free(second_array);
    free(last_array);

    return OK;
}

void rand_arr(int array[], int size) {
    for (int i = 0; i < size; ++i) {
        array[i] = -1000 + rand() % (1000 + 1000 + 1);
    }
}

int numcmp(const void *number1, const void *number2) {
    return (*(const int*)number1) - (*(const int*)number2);
}

int close_number(int number, int size, int array[]) {
    int left = 0, right = size - 1;

    while (left < right) {
        int middle = left + (right - left) / 2;

        int difference = array[middle] - number;

        if (difference == 0) {
            return array[middle];
        } else if (difference < 0) {
            left = middle + 1;
        } else {
            right = middle;
        }
    }

    if (left == 0) {
        return array[0];
    } else {
        if (abs(array[left] - number) < abs(array[left - 1] - number)) {
            return array[left];
        } else {
            return array[left - 1];
        }
    }
}

void print_array(int array[], int size) {
    for (int i = 0; i < size; ++i) {
        printf("%d ", array[i]);
    }
    printf("\n");
}