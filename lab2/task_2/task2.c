#include <stdio.h>
#include <math.h>
#include <float.h>
#include <stdarg.h>

typedef enum {
    OK = 0,
    OVERFLOW_ERROR = -1,
    INPUT_ERROR = -2,
} error;


error is_denormalize(const double *x) {
    if (*x != 0.0 && fabs(*x) < DBL_MIN) {
        return OVERFLOW_ERROR;
    }
    return OK;
}

error is_overflow(const double *x) {
    if (isinf(*x) || isnan(*x) || *x == 0.0) {
        return OVERFLOW_ERROR;
    }
    return OK;
}

void print_frame(int x) {
    int i;
    for (i = 0; i < x; ++i) {
        printf("=");
    }
    printf("\n");
}


error fast_pow(double base, int degree, double *pow_result) {
    error tester = OK;

    if (degree < 0) {
        return fast_pow(1.0 / base, -degree, pow_result);
    }

    if (degree == 0) {
        *pow_result = 1.0;
        return OK;
    }

    if ((degree & 1) == 0) {
        tester = fast_pow(base, degree / 2, pow_result);
        *pow_result *= *pow_result;
        if (is_denormalize(pow_result) != OK || is_overflow(pow_result) != OK) {
            return OVERFLOW_ERROR;
        }
    }

    if ((degree & 1) != 0) {
        tester = fast_pow(base, degree - 1, pow_result);
        *pow_result *= base;
        if (is_denormalize(pow_result) != OK || is_overflow(pow_result) != OK) {
            return OVERFLOW_ERROR;
        }
    }

    return tester;
}

error geometric_mean(double *result, unsigned int count, ...) {
    if (count == 0) {
        return INPUT_ERROR;
    }

    va_list args;
    va_start(args, count);

    for (int i = 0; i < count; ++i) {
        double x = va_arg(args, double);

        if (x <= 0) {
            va_end(args);
            return INPUT_ERROR;
        }
        if (is_overflow(&x) != OK || is_denormalize(&x) != OK ||
            is_overflow(result) != OK || is_denormalize(result) != OK) {
            va_end(args);
            return OVERFLOW_ERROR;
        }

        *result *= x;
    }
    va_end(args);

    *result = pow(*result, (double) (1.0 / count));
    return OK;
}


int main() {
    double result_pow = 1.0;
    double result_GM = 1.0;
    error result;

//    Тесты функции fast_pow
    print_frame(100);
    printf("TEST#1[fast_pow]\n");
    result_pow = 1.0;
    if (fast_pow(15, 20, &result_pow) == OVERFLOW_ERROR) {
        printf("ERROR: OVERFLOW_ERROR.\n");
        return OVERFLOW_ERROR;
    }
    printf("Result function 'fast_pow': %f\n", result_pow);
    printf("Result function 'pow': %f\n", pow(15, 20));

    print_frame(100);
    printf("TEST#2[fast_pow]\n");
    result_pow = 1.0;
    if (fast_pow(21.123, 5, &result_pow) == OVERFLOW_ERROR) {
        printf("ERROR: OVERFLOW_ERROR.\n");
        return OVERFLOW_ERROR;
    }
    printf("Result function 'fast_pow': %f\n", result_pow);
    printf("Result function 'pow': %f\n", pow(21.123, 5));

    print_frame(100);
    printf("TEST#3[fast_pow]\n");
    result_pow = 1.0;
    if (fast_pow(11.342, -4, &result_pow) == OVERFLOW_ERROR) {
        printf("ERROR: OVERFLOW_ERROR.\n");
        return OVERFLOW_ERROR;
    }
    printf("Result function 'fast_pow': %f\n", result_pow);
    printf("Result function 'pow': %f\n", pow(11.342, -4));

    print_frame(100);
    printf("TEST#4[fast_pow]\n");
    result_pow = 1.0;
    if (fast_pow(-121.111, 6, &result_pow) == OVERFLOW_ERROR) {
        printf("ERROR: OVERFLOW_ERROR.\n");
        return OVERFLOW_ERROR;
    }
    printf("Result function 'fast_pow': %f\n", result_pow);
    printf("Result function 'pow': %f\n", pow(-121.111, 6));

    print_frame(100);
    printf("TEST#5[fast_pow]\n");
    result_pow = 1.0;
    if (fast_pow(20.8437487324832784, 8, &result_pow) == OVERFLOW_ERROR) {
        printf("ERROR: OVERFLOW_ERROR.\n");
        return OVERFLOW_ERROR;
    }
    printf("Result function 'fast_pow': %f\n", result_pow);
    printf("Result function 'pow': %f\n", pow(20.8437487324832784, 8));

    print_frame(100);


//    Тесты функции geometric_mean
    print_frame(100);
    printf("TEST#1[geometric_mean]\n");

    result = geometric_mean(&result_GM, 2, 5.2, 52.1);
    if (result == OVERFLOW_ERROR) {
        printf("ERROR: OVERFLOW_ERROR.\n");
        return OVERFLOW_ERROR;
    }
    if (result == INPUT_ERROR) {
        printf("ERROR: INPUT_ERROR.\n");
        return INPUT_ERROR;
    }
    printf("Result function 'geometric_mean': %f\n", result_GM);
    result_GM = 1.0;


    print_frame(100);
    printf("TEST#2[geometric_mean]\n");

    result = geometric_mean(&result_GM, 5, 1.52, 2.52, 62.2, 61.1, 66.6);
    if (result == OVERFLOW_ERROR) {
        printf("ERROR: OVERFLOW_ERROR.\n");
        return OVERFLOW_ERROR;
    }
    if (result == INPUT_ERROR) {
        printf("ERROR: INPUT_ERROR.\n");
        return INPUT_ERROR;
    }
    printf("Result function 'geometric_mean': %f\n", result_GM);
    result_GM = 1.0;

    print_frame(100);
    printf("TEST#3[geometric_mean]\n");

    result = geometric_mean(&result_GM, 8, 1.52, 2.52, 1.2521, 61.1, 66.6, 0.11, 123.2, 9.1);
    if (result == OVERFLOW_ERROR) {
        printf("ERROR: OVERFLOW_ERROR.\n");
        return OVERFLOW_ERROR;
    }
    if (result == INPUT_ERROR) {
        printf("ERROR: INPUT_ERROR.\n");
        return INPUT_ERROR;
    }
    printf("Result function 'geometric_mean': %f\n", result_GM);
    result_GM = 1.0;


    return 0;
}