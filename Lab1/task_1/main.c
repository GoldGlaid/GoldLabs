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


// Функция возведения в степень
long long int artificial_pow(long long int x, int degree) {
    long long int num = 1;
    for (int i = 1; i <= degree; ++i) {
        if (num > LLONG_MAX / x) {
            return ERROR_OVERFLOW;
        }
        num *= x;
    }
    return num;
}

// Функция перевода из строчного представления в числовое, проверка на некоторые условия
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

int flag_H(long long int x) {
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

long long int flag_A(long long int x) {
    if (x == 0) {
        return ZERO_NUMBER;
    }
    if (x < 0) {
        return NEGATIVE_NUMBER;
    }
    if (x >= 960922000) {
        return ERROR_OVERFLOW;
    }
    return (1 + x) * x / 2;
}

void flag_S(long long int x, char string[]) {
    if (x < 0) {
        x *= -1;
    }
    sprintf(string, "%llx", x);
}


int flag_P(long long int x) {
    if (x == 0) {
        return ZERO_NUMBER;
    }
    if (x < 0) {
        return NEGATIVE_NUMBER;
    }

    if (x == 1) {
        return 1;
    }
    long long int b = round(sqrt(x));
    for (int i = 2; i < b; ++i) {
        if (x % i == 0) {
            return 1;
        }
    }
    return OK;
}

// Функция для подсчета факториала числа
long long int flag_F(long long int x) {
    if (x < 0) {
        return NEGATIVE_NUMBER;
    }

    long long int fact_num = 1;
    if (x == 0 || x == 1) {
        return fact_num;
    }
    for (long long int i = 1; i <= x; ++i) {
        if (fact_num > LLONG_MAX / i) {
            return ERROR_OVERFLOW;
        }
        fact_num *= i;
    }
    return fact_num;
}

int flag_E(long long int x) {
    if (x > 10) {
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
    long long int x;

    if (argc != 3) {
        printf("ERROR: INVALID_INPUT\n");
        return INVALID_INPUT;
    }
    // Проверка на верность введённого флага
    if (!(argv[1][0] == '-' || argv[1][0] == '/') && argv[1][2] == '\0') {
        printf("ERROR: INVALID_FLAG\n");
        return INVALID_FLAG;
    }
    // Проверка на верность введённого числа. Здесь происходит неявное переназначение переменной x!
    if (from_str_to_int(argv[2], &x, 10)) {
        printf("ERROR: INVALID_NUMBER\n");
        return INVALID_NUMBER;
    }

    // -h вывести в консоль натуральные числа в пределах 100 включительно, кратные x.
    // Если таковых нет – вывести соответствующее сообщение;
    if (argv[1][1] == 'h') {
        const int result = flag_H(x);
        if (result == INVALID_NUMBER) {
            printf("ERROR: INVALID_NUMBER, number can't be > 100\n");
            return INVALID_NUMBER;
        }
        if (result == ZERO_NUMBER) {
            printf("ERROR: INVALID_NUMBER. The number must be greater than 0\n");
            return ZERO_NUMBER;
        }
        if (result == NEGATIVE_NUMBER) {
            printf("ERROR: INVALID_NUMBER. The number must be non-negative\n");
            return NEGATIVE_NUMBER;
        }
        int a = 0;
        while (a + x <= 100) {
            a += x;
            printf("%d\n", a);
        }

        // Флаг -p определить, является ли число x простым; является ли x составным;
    } else if (argv[1][1] == 'p') {
        const int result = flag_P(x);
        if (result == ZERO_NUMBER) {
            printf("ERROR: INVALID_NUMBER. The number must be greater than 0\n");
            return ZERO_NUMBER;
        }
        if (result == NEGATIVE_NUMBER) {
            printf("ERROR: INVALID_NUMBER. The number must be non-negative\n");
            return NEGATIVE_NUMBER;
        }
        if (result) {
            printf("The number is not simple\n");
        } else {
            printf("The number is simple\n");
        }

        // Флаг -s разделить число x на отдельные цифры системы счисления с
        // основанием 16 и вывести отдельно каждую цифру числа, разделяя их
        // пробелом, от старших разрядов к младшим, без ведущих нулей в строковом
        // представлении;
    } else if (argv[1][1] == 's') {
        char string[100];
        flag_S(x, string);
        long long int len_hex_x = strlen(string);

        printf("Hex-num: ");
        if (x < 0) {
            printf("- ");
        }
        for (unsigned long long int i = 0; i < len_hex_x; ++i) {
            printf("%c ", string[i]);
        }
        printf("\n");

        // -e вывести таблицу степеней (для всех показателей в диапазоне от 1 до x)
        // оснований от 1 до 10; для этого флага работает ограничение на вводимое
        // число: x должен быть не больше 10;
    } else if (argv[1][1] == 'e') {
        int result = flag_E(x);
        if (result == INVALID_NUMBER) {
            printf("ERROR: INVALID_NUMBER, number can't be > 10\n");
            return INVALID_NUMBER;
        }
        if (result == ZERO_NUMBER) {
            printf("ERROR: INVALID_NUMBER. The number must be greater than 0\n");
            return ZERO_NUMBER;
        }
        if (result == NEGATIVE_NUMBER) {
            printf("ERROR: INVALID_NUMBER. The number must be non-negative\n");
            return NEGATIVE_NUMBER;
        }

        // Рамка
        for (int i = 0; i <= x * 12; ++i) {
            printf("=");
        }
        printf("\n");

        for (int i = 1; i <= x; ++i) {
            for (int j = 1; j <= x; ++j) {
                const long long int pow_i_j = artificial_pow(i, j);
                if (pow_i_j == ERROR_OVERFLOW) {
                    printf("ERROR: ERROR_OVERFLOW\n");
                    return ERROR_OVERFLOW;
                }
                printf("%10lli |", pow_i_j);
            }
            printf("\n");
        }

        // Рамка
        for (int i = 0; i <= x * 12; ++i) {
            printf("=");
        }
        printf("\n");

        // Флаг -a вычислить сумму всех натуральных чисел от 1 до x и вывести полученное значение в консоль;
    } else if (argv[1][1] == 'a') {
        const long long int result = flag_A(x);
        if (result == ZERO_NUMBER) {
            printf("ERROR: INVALID_NUMBER. The number must be greater than 0\n");
            return ZERO_NUMBER;
        }
        if (result == NEGATIVE_NUMBER) {
            printf("ERROR: INVALID_NUMBER. The number must be non-negative\n");
            return NEGATIVE_NUMBER;
        }
        if (result == ERROR_OVERFLOW) {
            printf("ERROR: ERROR_OVERFLOW\n");
            return ERROR_OVERFLOW;
        }
        printf("Sum: %lli\n", result);

        // Флаг -f вычислить факториал x и вывести полученное значение в консоль
    } else if (argv[1][1] == 'f') {
        const long long int result = flag_F(x);
        if (result == NEGATIVE_NUMBER) {
            printf("ERROR: INVALID_NUMBER. The number must be non-negative\n");
            return NEGATIVE_NUMBER;
        }
        if (result == ERROR_OVERFLOW) {
            printf("ERROR: ERROR_OVERFLOW\n");
            return ERROR_OVERFLOW;
        }
        printf("Factorial number: %lli\n", result);
    } else {
        printf("ERROR: INVALID_FLAG\n");
        return INVALID_FLAG;
    }
}
