#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <limits.h>
#include <float.h>

#define EPS DBL_EPSILON

typedef enum ERRORS{
    OK,
    INVALID_INPUT,
    NUMBER_OVERFLOW,
} error;

int final_representation (unsigned int base, unsigned long long int den);

unsigned long long int evklid_alg(unsigned long long int den, unsigned long long int num);

error fraction(unsigned long long int *answers, int *len_ans, unsigned int base, unsigned int count, ...);

error check_overflow_double(const double *number);

error make_denominator(unsigned int base, unsigned long long int *den, unsigned long long int *num, double numb);

int main() {

    unsigned long long int answers[100];
    int len_ans = 0;

    switch (fraction(answers, &len_ans, 2, 4, 0.25, 0.03, 0.03125, 0.07)) {
        case OK:

            for (int i = 0; i < len_ans; ++i) {
                if (answers[i] == 0){
                    printf("The number can be represented in the final representation\n");
                } else {
                    printf("The number cannot be represented in the final representation\n");
                }
            }
            break;
        case INVALID_INPUT:
            printf("Invalid input\n");
            return INVALID_INPUT;
        case NUMBER_OVERFLOW:
            printf("Number overflow\n");
            return NUMBER_OVERFLOW;
    }


    return OK;
}

error fraction(unsigned long long int *answers, int *len_ans, unsigned int base, unsigned int count, ...) {
    if (base < 2 || base > 36 || count == 0) return INVALID_INPUT;

    va_list args;
    va_start(args, count);

    for (int i = 0; i < count; ++i) {
        double number = va_arg(args, double);
        if (check_overflow_double(&number) != OK) return NUMBER_OVERFLOW;

        if (number <= 0 || number >= 1) {
            va_end(args);
            return INVALID_INPUT;
        }

        unsigned long long int numerator, denominator = 1;

        if (make_denominator(base, &denominator, &numerator, number) != OK) return NUMBER_OVERFLOW;

        if (final_representation(base, denominator) == 1) {
            answers[i] = denominator;
            *len_ans = *len_ans + 1;
        } else {
            answers[i] = 0;
            *len_ans = *len_ans + 1;
        }
    }

    va_end(args);
    return OK;
}

error check_overflow_double(const double *number) {
    if (isnan(*number) || isinf(*number)) {
        return NUMBER_OVERFLOW;
    } else {
        return OK;
    }
}

error make_denominator(unsigned int base, unsigned long long int *den, unsigned long long int *num, double numb) {
    while (fabs(numb - floor(numb)) > EPS) {
        if (*den > ULLONG_MAX / 10) return NUMBER_OVERFLOW;
        numb *= 10;
        *den *= 10;
    }

    *num = (unsigned long long int)numb;
    unsigned long long int alg = evklid_alg(*den, *num);

    *den /= alg;
    *num /= alg;

    return OK;
}

unsigned long long int evklid_alg(unsigned long long int den, unsigned long long int num) {
    while (den > 0 && num > 0) {
        if (den > num) {
            den %= num;
        } else {
            num %= den;
        }
    }

    if (den != 0) {
        return den;
    } else {
        return num;
    }
}

int final_representation(unsigned int base, unsigned long long int den) {
    for (int i = 2; i <= base; ++i) {
        if (i % 2 == 0 && i != 2) continue;

        while (base % i == 0 && den % i == 0) {
            while (base % i == 0) base /= i;
            while (den % i == 0) den /= i;

            if (den == 1) return 0;
        }
    }

    if (base == 1 && den == 1) {
        return 0;
    } else {
        return 1;
    }
}