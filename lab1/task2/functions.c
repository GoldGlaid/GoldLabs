#include "functions.h"


int str_to_double(char *str, double *number) {
    if (str == NULL || number == NULL) {
        return NULL_POINTER;
    }
    if (strlen(str) > 10) {
        return -3;
    }

    char *end;
    *number = strtod(str, &end);

    if (*number == HUGE_VAL || *number == -HUGE_VAL) {
        return -2;
    } else if (*number == 0 && end == str) {
        return -2;
    } else if (*end != '\0') {
        return -2;
    } else {
        return 0;
    }

}

double limit_e(double epsilon) {
    int n = 1;
    double current_iteration = 0.0, last_iteration;
    do {
        last_iteration = current_iteration;
        n += 1;
        current_iteration = pow((1.0 + 1.0 / n), n);
    } while (fabs(last_iteration - current_iteration) >= epsilon);
    return current_iteration;
}

double limit_pi(double epsilon) {
    int n = 2;
    double current_iteration = 4.0, last_iteration;
    do {
        last_iteration = current_iteration;
        current_iteration *= (4.0 * n * (n - 1.0)) / pow(2.0 * n - 1.0, 2);
        n++;
    } while (fabs(last_iteration - current_iteration) >= epsilon);
    return current_iteration;
}

double limit_ln2(double epsilon) {
    int n = 1;
    double current_iteration = 0.0, last_iteration;
    do {
        last_iteration = current_iteration;
        n *= 2;
        current_iteration = n * (pow(2.0, 1.0 / (double) n) - 1.0);
    } while (fabs(last_iteration - current_iteration) >= epsilon);
    return current_iteration;
}

double limit_sqrt2(double epsilon) {
    int n = 1;
    double current_iteration = 0.5, last_iteration;
    do {
        last_iteration = current_iteration;
        n++;
        current_iteration = last_iteration - last_iteration * last_iteration * 0.5 + 1.0;
    } while (fabs(last_iteration - current_iteration) >= epsilon);
    return current_iteration;
}

double limit_lambda(double epsilon) {
    int n = 1;
    double current_iteration = 1.0, last_iteration, sum = 1.0;
    do {
        last_iteration = current_iteration;
        n *= 2;

        for (int i = 2; i <= n; ++i) {
            sum += 1.0 / i;
        }
        current_iteration = sum - log(n);
        sum = 1.0;
    } while (fabs(last_iteration - current_iteration) >= epsilon);
    return current_iteration;
}

double row_e(double epsilon) {
    double sum = 1.0, last_iteration = 1.0;
    int n = 1;
    do {
        last_iteration /= (double) n;
        sum += last_iteration;
        n++;
    } while (last_iteration >= epsilon);

    return sum;
}

double row_pi(double epsilon) {
    double sum = 0.0, last_iteration;
    int n = 1, sign = 1;
    do {
        last_iteration = 4 * sign / (2.0 * n - 1.0);
        sum += last_iteration;
        n++;
        sign *= -1;
    } while (fabs(last_iteration) >= epsilon);

    return sum;
}

double row_ln2(double epsilon) {
    double sum = 0.0, last_iteration;
    int n = 1, sign = 1;

    do {
        last_iteration = (double) sign / n;
        sum += last_iteration;
        n++;
        sign *= -1;
    } while (fabs(last_iteration) >= epsilon);

    return sum;
}

double row_sqrt2(double epsilon) {
    double sum = pow(2.0, 0.25), last_iteration;
    int k = 2;

    do {
        last_iteration = sum;
        ++k;
        sum *= pow(2, 1.0 / pow(2, k));
    } while (fabs(sum - last_iteration) >= epsilon);

    return sum;
}

double row_lambda(double epsilon) {
    double current_iteration = 0.5, last_iteration = 0, sqrt_number;
    int k = 2;

    do {
        last_iteration = current_iteration;
        ++k;
        sqrt_number = sqrt(k);
        if (fmod(sqrt_number, 1.0) == 0) {
            k++;
            sqrt_number = (int) pow(k, 1.0 / 2.0);
        }
        current_iteration += 1.0 / pow((int) sqrt_number, 2.0) - 1.0 / k;
    } while (fabs(last_iteration - current_iteration) >= epsilon);

    return (current_iteration - pow(M_PI, 2) / 6);
}

double e_equation(double epsilon) {
    double x = 1;
    double f = 1, df;
    while (fabs(f) > epsilon) {
        f = log(x) - 1;
        df = 1.0 / x;
        x = x - f / df;
    }
    return x;
}

double pi_equation(double epsilon) {
    double x = 1;
    double f = 1, df;
    while (fabs(f) > epsilon) {
        f = cos(x) + 1;
        df = -sin(x);
        x = x - f / df;
    }
    return x;
}

double ln2_equation(double epsilon) {
    double x = 1;
    double f = 1, df;
    while (fabs(f) > epsilon) {
        f = exp(x) - 2;
        df = exp(x);
        x = x - f / df;
    }
    return x;
}

double sqrt2_equation(double epsilon) {
    double x = 1;
    double f = 1, df;
    while (fabs(f) > epsilon) {
        f = x * x - 2;
        df = 2 * x;
        x = x - f / df;
    }
    return x;
}

double lambda_equation(double epsilon) {
    double current_iteration = log(2) * 0.5, last_iteration, number = 0.5;
    int p = 2;

    do {
        last_iteration = current_iteration;
        do {
            ++p;
        } while (is_prime(p));
        number *= (p - 1.0) / p;
        current_iteration = log(p) * number;

    } while (fabs(last_iteration - current_iteration) >= epsilon);

    return (-log(current_iteration));
}

int is_prime(int number) {
    if (number < 0) number *= -1;

    if (number == 0 || number == 1 || number == 2) return 0;

    if (number % 2 == 0) return 1;

    for (int i = 3; i <= floor(sqrt(number)); i += 2) {
        if (number % i == 0) return 1;
    }
    return 0;
}