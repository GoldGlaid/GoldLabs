#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>

enum ERRORS{
    OK,
    INPUT_ERROR,
};

void swap(long long int numbers[], int size);

long long int str_to_llint(char* str, long long int* number, int system);

int main(int args, char *argv[]) {
    long long int a, b;
    srand(time(NULL));

    if (args != 3) {
        printf("ERROR: INPUT_ERROR.\n");
        return INPUT_ERROR;
    }



    if ((str_to_llint(argv[1], &a, 10) == -2)
        || (str_to_llint(argv[2], &b, 10) == -2)) {
        printf("ERROR: INPUT_ERROR.\n");
        return INPUT_ERROR;
    }

    if (a > b) {
        printf("ERROR: INPUT_ERROR.\n");
        return INPUT_ERROR;
    }

    long long int numbers[10];
    int size = sizeof(numbers) / sizeof(numbers[0]);

    for (int i = 0; i < size; ++i) {
        numbers[i] = a + rand() % (b - a + 1);
    }

    for (int i = 0; i < size; ++i) {
        printf("%lli ", numbers[i]);
    }
    printf("\n");

    swap(numbers, size);

    for (int i = 0; i < size; ++i) {
        printf("%lli ", numbers[i]);
    }
    printf("\n");

    return OK;
}

long long int str_to_llint(char* str, long long int* number, int system) {
    char* end = NULL;
    *number = strtoll(str, &end, system);
    if (*number == LLONG_MIN || *number == LLONG_MAX) {
        printf("the problem with the element\n");
        return -2;
    } else if (end == str && *number == 0) {
        printf("the problem with the element\n");
        return -2;
    } else if (*end != '\0') {
        printf("the problem with the element\n");
        return -2;
    }

    return 0;
}

void swap(long long int numbers[], int size) {

    long long int max_i = LLONG_MIN, min_i = LLONG_MAX;
    long long int count_max = -1, count_min = -1;

    for (int i = 0; i < size; ++i) {
        if (numbers[i] >= max_i) {
            max_i = numbers[i];
            count_max = i;
        }
        if (numbers[i] <= min_i) {
            min_i = numbers[i];
            count_min = i;
        }
    }

    if (count_max != -1 && count_min != -1) {
        long long int c;
        c = numbers[count_min];
        numbers[count_min] = numbers[count_max];
        numbers[count_max] = c;
    }
}