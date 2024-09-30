#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum STATE_ERRORS {
    OK,
    INVALID_INPUT,
    INVALID_FLAG,
    INVALID_NUMBER,
    ERROR_OVERFLOW
};

// Функция для проверки числа на простоту
int is_simple_digit(const int x) {
    if (x <= 0) {
        return 0;
    }
    for (int i = 2; i < x; ++i) {
        if (x % i == 0) {
            return 0;
        }
    }
    return 1;
}

// Функция возведения в степень
long long int artificial_pow(long long int x, int degree) {
    long long int num = 1;
    for (int i = 1; i <= degree; ++i) {
        if (num * x > LLONG_MAX) {
            return 0;
        }
        num *= x;
    }

    return num;
}

// Функция перевода из строчного представления в числовое, проверка на некоторые условия
int from_str_to_int(const char *argv, long long int *x, int base) {
    char *end_p;
    *x = strtol(argv, &end_p, base);
    if ((*x == LLONG_MAX || *x == LLONG_MIN) && (errno == ERANGE)) {
        return 0;
    }
    if (errno != 0 && *x == 0) {
        return 0;
    }
    if (*end_p != '\0') {
        return 0;
    }
    return 1;
}


// Функция для выполнения задания с флагом -s
void flag_S(long long int num) {
    char hex_num[100];
    sprintf(hex_num, "%llx", num);

    long long int len_hex_num = strlen(hex_num);

    printf("Hex-num: ");
    for (unsigned long long int i = 0; i < len_hex_num; ++i) {
        printf("%c ", hex_num[i]);
    }
    printf("\n");
}

// Функция для подсчета факториала числа
long long int factorial(long long int x) {
    long long int fact_num = 1;
    if (x == 0 || x == 1) {
        return fact_num;
    }
    for (long long int i = 1; i <= x; ++i) {
        if ((unsigned long long int) (fact_num * i) > LLONG_MAX) {
            return -1;
        }
        fact_num *= i;
    }
    return fact_num;
}


int main(int argc, char *argv[]) {
    long long int x;
    // Проверка на кол-во аргументов
    if (argc != 3) {
        printf("ERROR: INVALID_INPUT\n");
        return INVALID_INPUT;
    }
    // Проверка на верность введённого флага
    if (!(argv[1][0] == '-' || argv[1][0] == '/') && argv[1][2] == '\0') {
        printf("ERROR: INVALID_FLAG\n");
        return INVALID_FLAG;
    }
    // Проверка на верность введённого числа
    // Здесь происходит неявное переназначение переменной x!
    if (!from_str_to_int(argv[2], &x, 10)) {
        printf("ERROR: INVALID_NUMBER\n");
        return INVALID_NUMBER;
    }

    // Проверка на то что, число не равно 0 (не для флага f)
    if (x == 0 && argv[1][1] != 'f') {
        printf("ERROR: INVALID_NUMBER. The number must be greater than 0\n");
        return INVALID_NUMBER;
    }

    // Проверка на отрицательность числа
    if (x < 0) {
        printf("ERROR: INVALID_NUMBER. The number must be non-negative\n");
        return INVALID_NUMBER;
    }

    // -h вывести в консоль натуральные числа в пределах 100 включительно, кратные x.
    // Если таковых нет – вывести соответствующее сообщение;
    if (argv[1][1] == 'h') {
        enum flag {
            FALSE,
            TRUE
        };
        enum flag f = FALSE;

        if (x > 100) {
            printf("ERROR: INVALID_NUMBER, number can't be > 100\n");
            return INVALID_NUMBER;
        }

        for (int i = 1; i <= 100; i++) {
            if (i % x == 0) {
                printf("%d\n", i);
                f = TRUE;
            }
        }
        if (!f) {
            printf("No multiples of x were found\n");
        }


        // Флаг -p определить, является ли число x простым; является ли x составным;
    } else if (argv[1][1] == 'p') {
        if (is_simple_digit(x)) {
            printf("The number is simple\n");
        } else {
            printf("The number is not simple\n");
        }

        // Флаг -s разделить число x на отдельные цифры системы счисления с основанием 16 и
        // вывести отдельно каждую цифру числа, разделяя их пробелом, от старших
        // разрядов к младшим, без ведущих нулей в строковом представлении;
    } else if (argv[1][1] == 's') {
        flag_S(x);


        // -e вывести таблицу степеней (для всех показателей в диапазоне от 1 до x)
        // оснований от 1 до 10; для этого флага работает ограничение на вводимое число: x
        // должен быть не больше 10;
    } else if (argv[1][1] == 'e') {
        if (x > 10) {
            printf("ERROR: INVALID_NUMBER, number can't be > 10\n");
            return INVALID_NUMBER;
        }
        // Рамка
        for (int i = 0; i <= x * 12; ++i) {
            printf("=");
        }
        printf("\n");

        for (int i = 1; i <= x; ++i) {
            for (int j = 1; j <= x; ++j) {
                long long int result = artificial_pow(i, j);
                if (result) {
                    printf("%10lli |", result);
                } else {
                    printf("ERROR: INVALID_NUMBER\n");
                    return INVALID_NUMBER;
                }
            }
            printf("\n");
        }

        // Рамка
        for (int i = 0; i <= x * 12; ++i) {
            printf("=");
        }
        printf("\n");

        // Флаг -a вычислить сумму всех натуральных чисел от 1 до x и вывести полученное
        // значение в консоль;
    } else if (argv[1][1] == 'a') {
        long long int sum = 0;
        for (long long int i = 0; i <= x; ++i) {
            if ((unsigned long long int) (sum + i) > LLONG_MAX) {
                printf("ERROR: ERROR_OVERFLOW\n");
                return ERROR_OVERFLOW;
            }
            sum += i;
        }
        printf("Sum: %lli\n", sum);

        // Флаг -f вычислить факториал x и вывести полученное значение в консоль
    } else if (argv[1][1] == 'f') {
        long long int factorial_num = factorial(x);

        if (factorial_num == -1) {
            printf("ERROR: ERROR_OVERFLOW\n");
            return ERROR_OVERFLOW;
        }
        printf("Factorial number: %lli\n", factorial_num);
    } else {
        printf("ERROR: INVALID_FLAG\n");
        return INVALID_FLAG;
    }
}
