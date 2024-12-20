#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>

typedef enum STATES {
    OK,
    INVALID_INPUT,
    MEMORY_ERROR,
} state;

/*
 * 1) Пока полностью не раскроем многочлен:
 * - вычисляем f(a)
 * - берем производную по x
 * 2) gi = f^(i) (a) / i!
*/

double calc_equation(const double *coefs, int degree, double point);

void calc_df(const double *coefs, double *derivative_coefs, int degree);

state calculating_polynomial_coefs(double *array_answers, int degree, double point, double eps, ...);

int main() {
    int degree = 4;
    double a = 52.0;
    double eps = 1e-6;

    double *answers = (double *) malloc((degree + 1) * sizeof(double));
    if (answers == NULL) {
        printf("Memory error\n");
        return MEMORY_ERROR;
    }

    switch (calculating_polynomial_coefs(answers,
                                         degree,
                                         a,
                                         eps,
                                         -2.0, 1.0, -3.0, 0.0, 1.0)) {
        case MEMORY_ERROR:
            printf("ERROR: MEMORY_ERROR.\n");
            free(answers);
            return MEMORY_ERROR;
        case INVALID_INPUT:
            printf("ERROR: INVALID_INPUT.\n");
            free(answers);
            return INVALID_INPUT;
        case OK:
            printf("Answer f(x-a):\ngi = f^(i)(a) / i!\n");
            for (int i = 0; i <= degree; ++i) {
                printf("g%d = %lf\n", i, answers[i]);
            }
    }

    printf("\nOriginal polynomial values:\n");
    for (int x = 0; x <= degree; x++) {
        double f_x = -2.0 + 1.0 * x - 3.0 * x * x + 0.0 * x * x * x + 1.0 * x * x * x * x;
        printf("f(%d) = %lf\n", x, f_x);
    }

    printf("\nReconstructed polynomial values using Taylor expansion:\n");
    for (int x = 0; x <= degree; x++) {
        double g_x = 0.0;
        for (int i = 0; i <= degree; i++) {
            g_x += answers[i] * pow(x - a, i);
        }
        printf("g(%d) = %lf\n", x, g_x);
    }

    free(answers);
    return OK;
}

// Функция для вычисления значения многочлена в точке 'point' по коэффициентам 'coefs'.
double calc_equation(const double *coefs, int degree, double point) {
    double result = 0.0;
    for (int i = 0; i <= degree; ++i) {
        result += coefs[i] * pow(point, i); // Вычисление значения многочлена в точке
    }
    return result;
}

//Функция для вычисления производной многочлена и записи её коэффициентов
void calc_df(const double *coefs, double *derivative_coefs, int degree) {
    for (int i = 0; i < degree; ++i) {
        derivative_coefs[i] = coefs[i + 1] * (i + 1); // Коэффициенты производной: f'(x) = sum(a_n * n * x^(n-1))
    }
    derivative_coefs[degree] = 0.0; // Последний коэффициент производной становится 0
}

/**
 * Основная функция для вычисления коэффициентов разложения многочлена в ряд Тейлора.
 * Вычисляет коэффициенты g_i для разложения f(x) в точке 'point'.
 */
state calculating_polynomial_coefs(double *array_answers, int degree, double point, double eps, ...) {
    if (degree < 0 || array_answers == NULL || eps <= 0)
        return INVALID_INPUT;

    va_list args;
    va_start(args, eps);

    double *coefs = (double *) malloc((degree + 1) * sizeof(double));
    if (coefs == NULL) {
        va_end(args);
        return MEMORY_ERROR;
    }

    for (int i = 0; i <= degree; ++i) {
        coefs[i] = va_arg(args, double);
    }
    va_end(args);

    double *temp_coefs = (double *) malloc((degree + 1) * sizeof(double));
    if (temp_coefs == NULL) {
        free(coefs);
        return MEMORY_ERROR;
    }

    double prev_answer = 0.0; // Для отслеживания изменений коэффициентов
    // Вычисление коэффициентов разложения (где g_i = f^(i)(a) / i!)
    for (int i = 0; i <= degree; ++i) {
        double current_value = calc_equation(coefs, degree - i, point) / tgamma(i + 1); // i! через гамма-функцию
        array_answers[i] = current_value;

        // Проверка на изменение коэффициента относительно предыдущего значения
        if (fabs(current_value - prev_answer) < eps) {
            // Если разница меньше eps, то можно завершить вычисления
            break;
        }
        prev_answer = current_value;

        calc_df(coefs, temp_coefs, degree - i); // Вычисление производной
        for (int j = 0; j <= degree - i; ++j) {
            coefs[j] = temp_coefs[j]; // Обновление коэффициентов для следующей производной
        }
    }

    free(coefs);
    free(temp_coefs);
    return OK;
}
