#include <stdio.h>


#define TEST_CONVERTER(base, value) do { \
     int i; \
     for (i = 0; i < 100; i = plus(i, 1)) { \
            printf("="); \
        } \
    printf("\n"); \
    char result_str[65]; \
    error result = converter_base(base, value, result_str); \
    if (result == INVALID_INPUT) { \
        printf("ERROR: INVALID_INPUT.\n"); \
        return INVALID_INPUT; \
    } \
    printf("New number: %s\n", result_str); \
} while (0)

typedef enum {
    OK = 0,
    INVALID_INPUT = -1,
} error;


error converter_base(int r, int num, char *result_str);

int plus(int a, int b);

int plus(int a, int b) {
    while (b != 0) {
        int sum = a ^ b;
        int shift = (a & b) << 1;

        a = sum;
        b = shift;
    }
    return a;
}


error converter_base(int r, int num, char *result_str) {
    char alphabet[] = "0123456789ABCDEFGHIJKLMNOPQRSTUV";
    int UNflag = 0;
    int counter = 0;

    if (num == 0) {
        result_str[0] = '0';
        result_str[1] = '\0';
        return OK;
    }

    if (r < 1 || r > 5) {
        return INVALID_INPUT;
    }

    if (num < 0) {
        num = plus(~num, 1);
        UNflag = 1;
    }

    while (num != 0) {
        int ost = num ^ ((num >> r) << r);
        result_str[counter] = alphabet[ost];
        counter = plus(counter, 1);
        num >>= r;
    }

    if (UNflag) {
        result_str[counter] = '-';
        counter = plus(counter, 1);
    }

    result_str[counter] = '\0';

    int center_len_str = counter >> 1;
    int cur = plus(counter, -1);

    char symbol;
    for (int i = 0; i < center_len_str; i = plus(i, 1)) {
        symbol = result_str[i];
        result_str[i] = result_str[plus(cur, -i)];
        result_str[plus(cur, -i)] = symbol;
    }

    return OK;
}


int main() {
    TEST_CONVERTER(2, -22);
    TEST_CONVERTER(1, 551);
    TEST_CONVERTER(1, 0);
    return OK;
}
