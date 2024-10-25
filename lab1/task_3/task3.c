#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


typedef enum STATE_ERRORS {
    OK,
    INVALID_INPUT = -1,
    INVALID_FLAG __attribute__((unused)) = -2,
    INVALID_NUMBER = -3,
    ERROR_OVERFLOW = -4,
    ZERO_NUMBER = -5,
    NEGATIVE_NUMBER = -6,
} STATE_ERRORS;


struct value_case {
    long double x1;
    long double x2;
    bool exist;
    bool normal;
};


// Функция для перевода из строчного представления в числовое (целое), проверка некоторых условий
long long int str_to_int(const char *argv, long long int *x) {
    char *end_p;
    long long num = strtoll(argv, &end_p, 10);

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
    long double num = strtold(str, &end_p);

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

int x_multiple_y(long long int x, long long int y) {
    if (x == 0 || y == 0) {
        return ZERO_NUMBER;
    }
    if (x % y == 0) {
        return 1;
    }
    return 0;
}

int finding_the_sides_of_an_isosceles_triangle(long double a, long double b, long double c, long double eps) {
    if (!((fabsl(b) < HUGE_VALL / fabsl(b)) &&
        (fabsl(a) < HUGE_VALL / fabsl(a)) &&
        (fabsl(c) < HUGE_VALL / fabsl(c)) &&
        (fabsl(b * b) < HUGE_VALL - fabsl(c * c)) &&
        (fabsl(a * a) < HUGE_VALL - fabsl(c * c)) &&
        (fabsl(b * b) < HUGE_VALL - fabsl(a * a)))) {
        return ERROR_OVERFLOW;
    }

    if (isinf(a) || isinf(b) || isinf(c)) {
        return ERROR_OVERFLOW;
    }

    if ((fabsl(a) <= eps) || (fabsl(b) <= eps) || (fabsl(c) <= eps)) {
        return 0;
    }
    if ((a * a == b * b + c * c) || (b * b == a * a + c * c) || (c * c == b * b + a * a)) {
        return 1;
    } else {
        return 0;
    }
}

struct value_case func_quadratic_equation(long double a, long double b, long double c, long double epsilon) {
    struct value_case v_c = {0, 0, false, false};
    if (a == 0.0 || b == 0.0) {
        return v_c;
    }
    if (fabsl(a) < epsilon) {
        return v_c;
    }
    if (fabsl(b) < HUGE_VALL / fabsl(b) && fabsl(a) < HUGE_VALL / fabsl(c) && fabsl(a * c) < -HUGE_VALL / -4.0) {
        long double discriminant = (b * b) - 4.0 * a * c;

        if (discriminant > 0) {
            long double x1 = (-b + sqrtl(discriminant)) / (2.0 * a);
            long double x2 = (-b - sqrtl(discriminant)) / (2.0 * a);
            v_c.x1 = x1;
            v_c.x2 = x2;
            v_c.exist = true;
            v_c.normal = true;

        } else if (fabsl(discriminant) <= epsilon) {
            long double x = (-b) / (2.0 * a);
            v_c.x1 = x;
            v_c.x2 = x;
            v_c.exist = true;
            v_c.normal = true;

        } else {
            //комплексные корни
            v_c.exist = true;
            v_c.normal = false;
        }
    } else {
        // обработка переполнение
        v_c.x1 = HUGE_VALL;
        v_c.x2 = HUGE_VALL;
    }
    return v_c;
}


int main(int argc, char *argv[]) {
    if (!(argv[1][0] == '-' || argv[1][0] == '/') && argv[1][2] == '\0') {
        printf("ERROR: INVALID_FLAG.\n");
        return INVALID_FLAG;
    }

    switch (argv[1][1]) {
        case 'q':
            if (argc != 6) {
                printf("ERROR: INVALID_INPUT.\n");
                return INVALID_INPUT;
            }

            long double epsilon, a, b, c;
            if ((str_to_long_double(argv[2], &epsilon) ||
                 str_to_long_double(argv[3], &a) ||
                 str_to_long_double(argv[4], &b) ||
                 str_to_long_double(argv[5], &c)) != OK) {
                printf("ERROR: INVALID_NUMBER. You could have made a mistake when entering the numbers.\n");
                return INVALID_NUMBER;
            }

            if (epsilon < 0) {
                printf("ERROR: NEGATIVE_NUMBER. Epsilon must be non-negative.\n");
                return NEGATIVE_NUMBER;
            }
            long double pack[3] = {a, b, c};

            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    if (i == j) {
                        continue;
                    }
                    for (int k = 0; k < 3; ++k) {
                        if (i == k || j == k) {
                            continue;
                        }
                        //рамка
                        for (int l = 0; l <= 70; ++l) {
                            printf("=");
                        }
                        printf("\n");

                        printf("Combination of coefficients: a = %Lf, b = %Lf, c = %Lf\n", pack[i], pack[j], pack[k]);
                        struct value_case result = func_quadratic_equation(pack[i], pack[j], pack[k], epsilon);
                        if (!result.exist && !result.normal && result.x1 == 0 && result.x2 == 0) {
                            printf("ERROR: INVALID_NUMBER\n");
                            return INVALID_NUMBER;
                        }
                        if (!result.exist && !result.normal && result.x1 == HUGE_VALL && result.x2 == HUGE_VALL) {
                            printf("ERROR: ERROR_OVERFLOW.\n");
                            return ERROR_OVERFLOW;
                        }
                        if (!result.exist) {
                            printf("The equation is not square.\n");
                        } else if (!result.normal) {
                            printf("The equation has complex roots.\n");
                        } else {
                            printf("Roots: x1 = %.10Lf, x2 = %.10Lf\n", result.x1, result.x2);
                        }
                    }
                }
            }
            return OK;

        case 'm':
            if (argc != 4) {
                printf("ERROR: INVALID_INPUT.\n");
                return INVALID_INPUT;
            }
            long long int x, y;
            if (str_to_int(argv[2], &x) || str_to_int(argv[3], &y) != OK) {
                printf("ERROR: INVALID_NUMBER. You could have made a mistake when entering the numbers.\n");
                return INVALID_NUMBER;
            }

            long long int result = x_multiple_y(x, y);
            if (result == ZERO_NUMBER) {
                printf("ERROR: ZERO_NUMBER. The number must be greater than 0\n");
                return ZERO_NUMBER;
            }
            if (result) {
                printf("The number of %lld is a multiple of %lld.\n", x, y);
            } else {
                printf("The number of %lld is not a multiple of %lld.\n", x, y);
            }
            return OK;

        case 't':
            if (argc != 6) {
                printf("ERROR: INVALID_INPUT.\n");
                return INVALID_INPUT;
            }

            long double eps, num1, num2, num3;
            if ((str_to_long_double(argv[2], &eps) ||
                 str_to_long_double(argv[3], &num1) ||
                 str_to_long_double(argv[4], &num2) ||
                 str_to_long_double(argv[5], &num3)) != OK) {
                printf("ERROR: INVALID_NUMBER. You could have made a mistake when entering the numbers.\n");
                return INVALID_NUMBER;
            }

            if (eps < 0) {
                printf("ERROR: NEGATIVE_NUMBER. Epsilon must be non-negative.\n");
                return NEGATIVE_NUMBER;
            }

            int res = finding_the_sides_of_an_isosceles_triangle(num1, num2, num3, eps);

            if (res == ERROR_OVERFLOW) {
                printf("ERROR: ERROR_OVERFLOW.\n");
                return ERROR_OVERFLOW;
            }
            if (res) {
                printf("A = %.10Lf, B = %.10Lf, C = %.10Lf : are the sides of a right-angled triangle\n",
                       num1, num2, num3);
            } else {
                printf("A = %.10Lf, B = %.10Lf, C = %.10Lf : are not sides of a right-angled triangle\n",
                       num1, num2, num3);
            }
            return OK;

        default:
            printf("ERROR: INVALID_FLAG.\n");
            return INVALID_FLAG;
    }
}
