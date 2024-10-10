#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef enum STATE_ERRORS {
    OK,
    INVALID_INPUT = -1,
    INVALID_NUMBER = -3,
    ZERO_NUMBER = -5,
    NEGATIVE_NUMBER = -6,
} STATE_ERRORS;


// Функция для перевода из строчного представления в числовое (вещественное)
int str_to_long_double(const char *str, double *result) {
    int dot_flag = 0;

    for (int i = 0; i < strlen(str); ++i) {
        if ((str[i] < '0' || str[i] > '9') && str[i] != '.' && str[i] != '-') {
            return INVALID_NUMBER;
        }
        if (str[i] == '-' && i != 0) {
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


double integral_a(double x) {
    return log(1 + x) / x;
}

double integral_b(double x) {
    return exp(-(x * x / 2));
}

double integral_c(double x) {
    return log(1.0 / (1.0 - x));
}

double integral_d(double x) {
    return pow(x, x);
}


double rectangle_method(int n, double epsilon, double (*function)(double)) {
    double sum = 0.0, interval = 1.0 / n, x, i;

    for (i = epsilon; i < n - epsilon; i++) {
        x = interval * i;
        sum += function(x);
    }
    return sum * interval;
}

double func_integrate(double epsilon, double (*function)(double)) {
    double result = 0.0, prev_result = 1.0;
    int n = 1;

    while (fabs( result - prev_result) >= epsilon) {
        prev_result = result;
        result = rectangle_method(n, epsilon, function);
        n *= 2;
    }

    return result;
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("ERROR: INVALID_INPUT.\n");
        return INVALID_INPUT;
    }

    double epsilon;
    if (str_to_long_double(argv[1], &epsilon) != OK) {
        printf("ERROR: INVALID_NUMBER. You could have made a mistake when entering the epsilon.\n");
        return INVALID_NUMBER;
    }
    if (epsilon < 0) {
        printf("ERROR: NEGATIVE_NUMBER. Epsilon must be non-negative.\n");
        return NEGATIVE_NUMBER;
    }
    if (epsilon == 0) {
        printf("ERROR: ZERO_NUMBER. The Epsilon must be greater than 0\n");
        return ZERO_NUMBER;
    }
    if (epsilon > 1) {
        printf("ERROR: INVALID_NUMBER. The epsilon is to big\n");
        return INVALID_NUMBER;
    }

    printf("Integral a: %lf\n", func_integrate(epsilon, integral_a));
    printf("Integral b: %lf\n", func_integrate(epsilon, integral_b));
    printf("Integral c: %lf\n", func_integrate(epsilon, integral_c));
    printf("Integral d: %lf\n", func_integrate(epsilon, integral_d));

}
