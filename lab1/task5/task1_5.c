#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>

int str_to_long_double(const char *str, double *result);

double func_a(double x, double epsilon);

double func_b(double x, double epsilon);

double func_c(double x, double epsilon);

double func_d(double x, double epsilon);

int is_inf(double x);

enum ERRORS {
    OK,
    INVALID_NUMBER,
    INVALID_INPUT,
    PROBLEMS_WITH_EPS,
};

int main(int args, char *argv[]) {
    double x;
    double epsilon;

    if (args != 3) {
        printf("ERROR: INVALID_INPUT.\n");
        return INVALID_INPUT;
    }

    if (str_to_long_double(argv[1], &x) == INVALID_NUMBER) {
        printf("ERROR: INVALID_NUMBER.\n");
        return INVALID_NUMBER;
    }

    if (str_to_long_double(argv[2], &epsilon) == INVALID_NUMBER || epsilon <= 0 || epsilon > 1) {
        printf("ERROR: PROBLEMS_WITH_EPS.\n");
        return PROBLEMS_WITH_EPS;
    }

    if (func_a(x, epsilon) != epsilon) {
        printf("A: %lf\n", func_a(x, epsilon));
    } else printf("A: Invalid input for that function.\n");
    if (func_b(x, epsilon) != epsilon) {
        printf("B: %lf\n", func_b(x, epsilon));
    } else printf("B: Invalid input for that function.\n");
    if (func_c(x, epsilon) != epsilon) {
        printf("C: %lf\n", func_c(x, epsilon));
    } else printf("C: Invalid input for that function.\n");
    if (func_d(x, epsilon) != epsilon) {
        printf("D: %lf\n", func_d(x, epsilon));
    } else printf("D: Invalid input for that function.\n");

    return OK;
}

// Функция для перевода из строчного представления в числовое (вещественное), проверка некоторых условий
int str_to_long_double(const char *str, double *result) {
    int dot_flag = 0;

    for (int i = 0; i < strlen(str); ++i) {
        if ((str[i] < '0' || str[i] > '9') && str[i] != '.' || (i != 0 && str[i] == '-')) {
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
    double num = strtod(str, &end_p);

    if (num == HUGE_VAL || num == -HUGE_VAL) {
        return INVALID_NUMBER;
    }
    if (num == 0 && end_p == str) {
        return INVALID_NUMBER;
    }
    if (*end_p != '\0') {
        return INVALID_NUMBER;
    }
    *result = num;
    return OK;
}

double func_a(double x, const double epsilon) {
    int n = 0;
    double result = 0, iter_answer = 1.0;

    while (fabs(iter_answer) >= epsilon) {
        if (n == INT_MAX) {
            return epsilon;
        }
        result += iter_answer;
        ++n;
        iter_answer *= x / n;
    }

    return result;
}

double func_b(double x, const double epsilon) {
    int n = 0;
    double result = 0, iter_answer = 1.0;

    while (fabs(iter_answer) >= epsilon) {
        if (n == INT_MAX) {
            return epsilon;
        }
        result += iter_answer;
        ++n;
        iter_answer *= -1.0 * x * x / ((2 * n) * (2 * n - 1.0));
    }

    return result;
}

double func_c(double x, const double epsilon) {
    int n = 0;
    double result = 0, iter_answer = 1.0;

    while (fabs(iter_answer) >= epsilon) {

        if (n == INT_MAX) {
            return epsilon;
        }
        result += iter_answer;
        ++n;
        iter_answer *= 9.0 * x * x * n * n / (3.0 * n * (3.0 * n - 3.0) + 2.0);

        if (epsilon + iter_answer >= 1.0) {
            return INFINITY;
        }
    }

    return result;
}

double func_d(double x, const double epsilon) {
    int n = 0;
    double result = 0;
    double iter_answer = -1.0 * x * x / 2.0;

    if (fabs(x) >= 1) {
        return INFINITY;
    }

    while (fabs(iter_answer) >= epsilon) {
        if (is_inf(result) == 1 || is_inf(iter_answer) == 1) {
            return INFINITY;
        }
        if (n == INT_MAX) {
            return epsilon;
        }
        result += iter_answer;
        ++n;
        iter_answer *= (-1.0 * x * x * (2.0 * n - 1.0)) / (2.0 * n);
    }

    return result;
}

int is_inf(double x) {
    if (isinf(x) || fabs(x) > DBL_MAX) {
        return 1;
    }
    return 0;
}