#include <inttypes.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum STATE_ERRORS {
    OK,
    INVALID_INPUT = -1,
    INVALID_FLAG = -2,
    INVALID_NUMBER = -3,
    ERROR_OVERFLOW = -4,
    ZERO_NUMBER = -5,
    NEGATIVE_NUMBER = -6,
} STATE_ERRORS;


// Функция для перевода из строчного представления в числовое (целое), проверка некоторых условий
int from_str_to_int(const char *argv, long long int *x, int base) {
    char *end_p;
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

// Функция для перевода из строчного представления в числовое (вещественное), проверка некоторых условий
int str_to_long_double(const char *str, long double *result) {
    int dot_flag = 0;

    for (int i = 0; i < strlen(str); ++i) {
        if ((str[i] < '0' || str[i] > '9') && str[i] != '.') {
            return INVALID_NUMBER;
        }
        if (str[i] == '.') {
            dot_flag++;
        }
    }
    if (dot_flag != 1) {
        return INVALID_NUMBER;
    }

    char *end_p;
    long double num = strtold(str, &end_p);

    if (num == HUGE_VAL || num == -HUGE_VAL) {
        return INVALID_NUMBER;
    } else if (num == 0 && end_p == str) {
        return INVALID_NUMBER;
    } else if (*end_p != '\0') {
        return INVALID_NUMBER;
    }
    *result = num;
    return OK;
}

int flag_Q(long long int x) {
    if (x > 100) {
        return INVALID_NUMBER;
    }
    if (x == 0) {
        return ZERO_NUMBER;
    }
    if (x < 0) {
        return NEGATIVE_NUMBER;
    }
    return OK;
}


int flag_M(long long int x) {
    if (x > 100) {
        return INVALID_NUMBER;
    }
    if (x == 0) {
        return ZERO_NUMBER;
    }
    if (x < 0) {
        return NEGATIVE_NUMBER;
    }
    return OK;
}

int flag_T(long long int x) {
    if (x > 100) {
        return INVALID_NUMBER;
    }
    if (x == 0) {
        return ZERO_NUMBER;
    }
    if (x < 0) {
        return NEGATIVE_NUMBER;
    }
    return OK;
}


int main(int argc, char *argv[]) {

    if (!(argv[1][0] == '-' || argv[1][0] == '/') && argv[1][2] == '\0') {
        printf("ERROR: INVALID_FLAG\n");
        return INVALID_FLAG;
    }

    switch (argv[1][1]) {
        case 'q':
            if (argc != 6) {
                printf("ERROR: INVALID_INPUT\n");
                return INVALID_INPUT;
            }
        //TODO /q 1.1 2 3 4 нету ошибки, нужно исправить

            long double epsilon, a, b, c;
            if ((str_to_long_double(argv[2], &epsilon) &&
                str_to_long_double(argv[3], &a) &&
                str_to_long_double(argv[4], &b) &&
                str_to_long_double(argv[5], &c)) != OK) {
                printf("ERROR: INVALID_NUMBER. You could have made a mistake when entering the numbers\n");
                return INVALID_INPUT;
            }
            if (epsilon < 0) {
                printf("ERROR: INVALID_NUMBER. Epsilon must be non-negative\n");
                return INVALID_NUMBER;
            }
        case 'm':
            break;
        case 't':
            break;
        default:
            printf("ERROR: INVALID_FLAG\n");
            return INVALID_FLAG;;
    }
    // // Проверка на верность введённого числа. Здесь происходит неявное переназначение переменной x!
    // if (from_str_to_int(argv[2], &x, 10)) {
    //     printf("ERROR: INVALID_NUMBER\n");
    //     return INVALID_NUMBER;
    // }

    // -h вывести в консоль натуральные числа в пределах 100 включительно, кратные x.
    // Если таковых нет – вывести соответствующее сообщение;
    // if (argv[1][1] == 'h') {
    //     const int result = flag_H(x);
    //     if (result == INVALID_NUMBER) {
    //         printf("ERROR: INVALID_NUMBER, number can't be > 100\n");
    //         return INVALID_NUMBER;
    //     }
    //     if (result == ZERO_NUMBER) {
    //         printf("ERROR: INVALID_NUMBER. The number must be greater than 0\n");
    //         return ZERO_NUMBER;
    //     }
    //     if (result == NEGATIVE_NUMBER) {
    //         printf("ERROR: INVALID_NUMBER. The number must be non-negative\n");
    //         return NEGATIVE_NUMBER;
    //     }
    //     int a = 0;
    //     while (a + x <= 100) {
    //         a += x;
    //         printf("%d\n", a);
    //     }

        // Флаг -p определить, является ли число x простым; является ли x составным;
    // } else if (argv[1][1] == 'p') {
    //     const int result = flag_P(x);
    //     if (result == ZERO_NUMBER) {
    //         printf("ERROR: INVALID_NUMBER. The number must be greater than 0\n");
    //         return ZERO_NUMBER;
    //     }
    //     if (result == NEGATIVE_NUMBER) {
    //         printf("ERROR: INVALID_NUMBER. The number must be non-negative\n");
    //         return NEGATIVE_NUMBER;
    //     }
    //     if (result) {
    //         printf("The number is not simple\n");
    //     } else {
    //         printf("The number is simple\n");
    //     }
    //     // Флаг -a вычислить сумму всех натуральных чисел от 1 до x и вывести полученное значение в консоль;
    // } else if (argv[1][1] == 'a') {
    //     const long long int result = flag_A(x);
    //     if (result == ZERO_NUMBER) {
    //         printf("ERROR: INVALID_NUMBER. The number must be greater than 0\n");
    //         return ZERO_NUMBER;
    //     }
    //     if (result == NEGATIVE_NUMBER) {
    //         printf("ERROR: INVALID_NUMBER. The number must be non-negative\n");
    //         return NEGATIVE_NUMBER;
    //     }
    //     if (result == ERROR_OVERFLOW) {
    //         printf("ERROR: ERROR_OVERFLOW\n");
    //         return ERROR_OVERFLOW;
    //     }
    //     printf("Sum: %lli\n", result);
    // }
}
