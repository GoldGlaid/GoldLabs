#include <stdio.h>
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>


typedef enum STATES {
    OK,
    INVALID_INPUT,
    NUMBER_OVERFLOW,
    UNKNOWN_ERROR,
} state;

typedef struct Point {
    double x;
    double y;
} Point;

state polynomial(double *answer, double point, unsigned int degree, ...);

state check_overflow_double(const double *x);

state string_to_ulint(const char *str, unsigned long int *res, int base);

state pow_ulint(unsigned long int *res);

void ulint_to_your_base(char *result, unsigned long int res, int base);

state check_sum_strings(unsigned long int *answer_array, int count, char *original, unsigned long int number,
                        int base);

state check_Kaprekara_numbers(unsigned long int *answer_array, int base, int count, ...);

int Bulge(int count, long double eps, ...);

int vector_product(Point p1, Point p2, Point p3, long double eps);

int my_strlen(const char *str);

int main() {
    int count = 4, i;
    int counts_of_points = 4;
    unsigned long int answer_array[100];
    double answer;

    long double eps = 1e-7;

    Point p1 = {0, 0};
    Point p2 = {1, 0};
    Point p3 = {0, 1};
    Point p4 = {0, 1};

    //Проверка на выпуклость многоугольника
    printf("Bulge: \n");
    if (Bulge(counts_of_points, eps, p1, p2, p3, p4) == 0) {
        printf("The polygon is convex\n");
    } else {
        printf("The polygon is NOT convex\n");
    }

    //Нахождение значения многочлена степени n в заданной точке
    printf("\nPolynomial: \n");
    switch (polynomial(&answer, 1.5, 2, 2.20, 10.3, 15.65)) {
        case OK:
            printf("Answer: %f\n", answer);
            break;
        case NUMBER_OVERFLOW:
            printf("ERROR: NUMBER_OVERFLOW.\n");
            return NUMBER_OVERFLOW;
        case INVALID_INPUT:
            printf("ERROR: INVALID_INPUT.\n");
            return INVALID_INPUT;
        default:
            printf("ERROR: UNKNOWN_ERROR.\n");
            return UNKNOWN_ERROR;
    }


    //Нахождение чисел Капрекара
    /*
     * Сводка:
     * Число Капрекара
     * Это неотрицательное целое число, квадрат которого в этой системе можно разбить на две части,
     * сумма которых даёт исходное число.
     * Например, 45 является числом Капрекара, поскольку 45² = 2025 и 20 + 25 = 45.
     */

    printf("\nKaprekar: \n");
    switch (check_Kaprekara_numbers(answer_array, 10, count, "45", "297", "703", "100")) {
        case OK:
            for (i = 0; i < count * 2; i += 2) {
                if (answer_array[i + 1] == 0) {
                    printf("The number %lu is the Kaprekar number\n", answer_array[i]);
                } else {
                    printf("The number %lu is NOT the Kaprekar number\n", answer_array[i]);
                }
            }
            break;
        case NUMBER_OVERFLOW:
            printf("ERROR: NUMBER_OVERFLOW.\n");
            return NUMBER_OVERFLOW;
        case INVALID_INPUT:
            printf("ERROR: INVALID_INPUT.\n");
            return INVALID_INPUT;
        default:
            printf("ERROR: UNKNOWN_ERROR.\n");
            return UNKNOWN_ERROR;
    }
    return OK;
}

state polynomial(double *answer, double point, unsigned int degree, ...) {
    if (check_overflow_double(&point) != OK) return NUMBER_OVERFLOW;

    va_list args;
    va_start(args, degree);
    double coef = va_arg(args, double), gorner;
    unsigned int i;
    if (check_overflow_double(&coef) != OK) {
        va_end(args);
        return NUMBER_OVERFLOW;
    }
    gorner = coef;

    for (i = degree; i > 0; --i) {
        coef = va_arg(args, double);
        if (check_overflow_double(&coef) != OK) {
            va_end(args);
            return NUMBER_OVERFLOW;
        }

        gorner = gorner * point + coef;
        if (check_overflow_double(&gorner) != OK) {
            va_end(args);
            return NUMBER_OVERFLOW;
        }
    }

    *answer = gorner;
    va_end(args);
    return OK;
}

state check_overflow_double(const double *x) {
    if (isnan(*x) || isinf(*x)) {
        return NUMBER_OVERFLOW;
    } else {
        return OK;
    }
}

state string_to_ulint(const char *str, unsigned long int *res, int base) {
    char *end = NULL;
    *res = strtoul(str, &end, base);

    if (*res > ULONG_MAX) {
        return NUMBER_OVERFLOW;
    } else if (*res == 0 && end == str) {
        return INVALID_INPUT;
    } else if (*end != '\0') {
        return INVALID_INPUT;
    }

    return OK;
}

state pow_ulint(unsigned long int *res) {
    unsigned long int square = (*res) * (*res);
    if (square / (*res) != (*res)) {
        return NUMBER_OVERFLOW;
    }
    *res = square;
    return OK;
}


void ulint_to_your_base(char *result, unsigned long int res, int base) {
    int count = 0, num, i;
    char swap;
    while (res > 0) {
        num = res % base;
        if (num > 9) {
            result[count] = num + 'A' - 10;
        } else {
            result[count] = num + '0';
        }

        res /= base;
        ++count;
    }

    for (i = 0; i < count / 2; ++i) {
        swap = result[i];
        result[i] = result[count - i - 1];
        result[count - i - 1] = swap;
    }

    result[count] = '\0';
}

int my_strlen(const char *str) {
    char const *end = str;
    while (*end++);
    return end - str - 1;
}

state check_sum_strings(unsigned long int *answer_array, int count, char *original, unsigned long int number,
                        int base) {
    int len = my_strlen(original), i;
    original[len] = '\0';
    char new[100];
    unsigned long int num1, num2;

    for (i = 0; i < len; ++i) {
        new[i] = original[i];
        new[i + 1] = '\0';
        original[i] = '0';

        if (string_to_ulint(new, &num1, base) == OK
            && string_to_ulint(original, &num2, base) == OK) {
            if (num1 + num2 == number && num1 != 0 && num2 != 0) {
                answer_array[count + 1] = 0;
                return OK;
            }
        } else {
            return INVALID_INPUT;
        }
    }

    answer_array[count + 1] = 1;
    return OK;
}

state check_Kaprekara_numbers(unsigned long int *answer_array, int base, int count, ...) {
    if (base < 2 || base > 36 || count < 1) return INVALID_INPUT;

    int i;
    unsigned long int new;
    va_list args;
    va_start(args, count);
    char number_line[BUFSIZ];
    unsigned long int number;

    for (i = 0; i < count; ++i) {
        // считываю число, закидываю в переменную, перевожу в uns long int
        strcpy(number_line, va_arg(args, char *));
        if (string_to_ulint(number_line, &number, base) != OK) {
            va_end(args);
            return INVALID_INPUT;
        }
        /*
         * делаю 2*i с идеей того, что в i элементе будет написано преобразовалось ли число, а в 2*i само число
         * + возведение в квадрат
        */
        answer_array[2 * i] = number;
        new = number;
        if (pow_ulint(&number) != OK) {
            va_end(args);
            return NUMBER_OVERFLOW;
        }
        // Перевожу число в квадрате в строчку, чтобы начать разбивать
        ulint_to_your_base(number_line, number, base);

        // Тут вся дробежка + переводы и сравнения
        if (check_sum_strings(answer_array, 2 * i, number_line, new, base) != OK) {
            va_end(args);
            return INVALID_INPUT;
        }
    }

    va_end(args);
    return OK;
}

int Bulge(int count, long double eps, ...) {
    if (count < 3) return 1;

    int i;
    va_list args;
    double answer;
    va_start(args, eps);

    Point point1, point2;

    Point p1 = va_arg(args, Point);
    Point p2 = va_arg(args, Point);
    Point p3 = va_arg(args, Point);

    answer = vector_product(p1, p2, p3, eps);

    point1 = p1;
    point2 = p2;

    for (i = 0; i < count - 3; ++i) {
        point1 = point2;
        point2 = p3;
        p3 = va_arg(args, Point);

        if (vector_product(point1, point2, p3, eps) != answer) {
            va_end(args);
            return 1;
        }
    }

    if (vector_product(point2, p3, point1, eps) != answer
        || vector_product(p3, point1, p1, eps) != answer) {
        va_end(args);
        return 1;
    }

    va_end(args);
    return 0;
}

int vector_product(Point p1, Point p2, Point p3, long double eps) {
    double cross = (p2.x - p1.x) * (p3.y - p2.y) - (p2.y - p1.y) * (p3.x - p2.x);
    if (fabs(cross) < eps) {
        return 0; // Точки лежат на одной прямой
    }
    return (cross > 0) ? 1 : -1;
}