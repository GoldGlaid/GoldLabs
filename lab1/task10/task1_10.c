#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


typedef enum STATE_ERRORS {
    OK,
    INVALID_BASE = -2,
    INVALID_NUMBER = -3,
} STATE_ERRORS;


// Функция перевода из строчного представления в числовое, проверка на некоторые условия
long long int str_to_int(const char *argv, long long int *x, int base) {
    char *end_p;
    int i;
    for (i = 0; i < strlen(argv); ++i) {
        if (argv[i] >= 'a' && argv[i] <= 'z') {
            return INVALID_NUMBER;
        }
    }
    long long num = strtoll(argv, &end_p, base);

    if (num >= LLONG_MAX || num <= LLONG_MIN) {
        return INVALID_NUMBER;
    }
    if (*end_p != '\0') {
        return INVALID_NUMBER;
    }
    *x = num;
    return OK;
}


int convert_to_base(char *str, long long int x, int base) {
    char alphabet[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int maxbase = sizeof(alphabet) - 1;
    bool negative_flag = false;
    char *p_str = str;

    if (base > maxbase) {
        return INVALID_BASE;
    }

    if (x == 0) {
        *(p_str++) = alphabet[0];
    }

    if (x < 0) {
        negative_flag = true;
        x = llabs(x);
    }

    while (x != 0) {
        *(p_str++) = alphabet[x % base];
        x /= base;
    }

    if (negative_flag) {
        *(p_str++) = '-';
    }

    *p_str = '\0';
    _strrev(str);

    return OK;

}


int main() {
    int base_of_system; // [2,...,36]
    int result = scanf("%d", &base_of_system);
    bool checker_nums = false;

    if (result != 1 || base_of_system < 2 || base_of_system > 36) {
        printf("ERROR: INVALID_BASE.\n");
        return INVALID_BASE;
    }

    long long int x = 0, max = 0;
    char str[65];

    scanf("%s", str);

    while (strcmp(str, "Stop") != 0) {
        checker_nums = true;
        if (str_to_int(str, &x, base_of_system)) {
            printf("ERROR: INVALID_NUMBER.\n");
            return INVALID_NUMBER;
        }
        if (llabs(max) < llabs(x)) {
            max = x;
        }
        scanf("%s", str);
    }

    if (!checker_nums) {
        printf("ERROR: INVALID_NUMBER. Please enter at least one more number\n");
        return INVALID_NUMBER;
    }
    char answer[65];

    convert_to_base(answer, max, base_of_system);
    printf("The numbers in the entered base %d: %s\n", base_of_system, answer);
    convert_to_base(answer, max, 9);
    printf("Base 9: %s\n", answer);
    convert_to_base(answer, max, 18);
    printf("Base 18: %s\n", answer);
    convert_to_base(answer, max, 27);
    printf("Base 27: %s\n", answer);
    convert_to_base(answer, max, 36);
    printf("Base 36: %s\n", answer);

    return OK;
}