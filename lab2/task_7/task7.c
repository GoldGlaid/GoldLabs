#include <stdio.h>
#include <math.h>

typedef enum {
    OK = 0,
    ONE_SIGN = -2,
    PROBLEM_WITH_EPSILON = -3,
} error;

error dichotomy(double *res, double a, double b, double eps, double (*f)(double)) {
    if (eps < 1e-10 || eps <= 0) {
        return PROBLEM_WITH_EPSILON;
    }
    if (fabs(f(a)) <= eps) {
        *res = a;
        return OK;
    }
    if (fabs(f(b)) <= eps) {
        *res = b;
        return OK;
    }
    if (f(a) * f(b) > 0) {
        return ONE_SIGN;
    }
    double mid = (a + b) / 2;
    while (fabs(f(mid)) >= eps) {
        if (f(a) * f(b) > 0) {
            return ONE_SIGN;
        }
        mid = (a + b) / 2;
        if (f(mid) * f(a) < 0) {
            b = mid;
        }
        if (f(mid) * f(b) < 0) {
            a = mid;
        }
    }

    *res = mid;
    return OK;
}

double func1(double x) {
    return x * x - 4;
}

double func2(double x) {
    return x * x * x - 1;
}

double func3(double x) {
    return exp(x) - 2;
}

double func4(double x) {
    return sin(x);
}

double func5(double x) {
    return log(x) - 1;
}

int main() {
    double result = 0.0;
//    Ожидаемый результат: около 2
    if (dichotomy(&result, 0, 3, 0.01, func1) == OK) {
        printf("%f\n", result);
    } else if (dichotomy(&result, 0, 3, 0.01, func1) == ONE_SIGN) {
        printf("ERROR: ONE_SIGN\n");
    } else {
        printf("ERROR: PROBLEM_WITH_EPSILON\n");
    }
    result = 0.0;

    //    Ожидаемый результат: около -2
    if (dichotomy(&result, -3, 0, 0.01, func1) == OK) {
        printf("%f\n", result);
    } else if (dichotomy(&result, -3, 0, 0.01, func1) == ONE_SIGN) {
        printf("ERROR: ONE_SIGN\n");
    } else {
        printf("ERROR: PROBLEM_WITH_EPSILON\n");
    }
    result = 0.0;

//    Ожидаемый результат: около 1
    if (dichotomy(&result, 0, 2, 0.01, func2) == OK) {
        printf("%f\n", result);
    } else if (dichotomy(&result, 0, 2, 0.01, func2) == ONE_SIGN) {
        printf("ERROR: ONE_SIGN\n");
    } else {
        printf("ERROR: PROBLEM_WITH_EPSILON\n");
    }
    result = 0.0;

    //    Ожидаемый результат: около 0.693
    if (dichotomy(&result, 0, 2, 0.001, func3) == OK) {
        printf("%f\n", result);
    } else if (dichotomy(&result, 0, 2, 0.001, func3) == ONE_SIGN) {
        printf("ERROR: ONE_SIGN\n");
    } else {
        printf("ERROR: PROBLEM_WITH_EPSILON\n");
    }
    result = 0.0;

    //    Ожидаемый результат: около 0
    if (dichotomy(&result, -1, 1, 0.001, func4) == OK) {
        printf("%f\n", result);
    } else if (dichotomy(&result, -1, 1, 0.001, func4) == ONE_SIGN) {
        printf("ERROR: ONE_SIGN\n");
    } else {
        printf("ERROR: PROBLEM_WITH_EPSILON\n");
    }
    result = 0.0;

    //    Ожидаемый результат: около 2.718.
    if (dichotomy(&result, 2, 4, 0.001, func5) == OK) {
        printf("%f\n", result);
    } else if (dichotomy(&result, 2, 4, 0.001, func5) == ONE_SIGN) {
        printf("ERROR: ONE_SIGN\n");
    } else {
        printf("ERROR: PROBLEM_WITH_EPSILON\n");
    }
    result = 0.0;

    return OK;
}
