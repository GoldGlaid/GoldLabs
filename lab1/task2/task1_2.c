#include "functions.h"

int main(int args, char *argv[]) {
    double epsilon;

    if (args != 2) {
        printf("ERROR: INVALID_INPUT.\n");
        return INVALID_INPUT;
    }
    if (str_to_double(argv[1], &epsilon) == -2 || epsilon <= 0.0 || epsilon >= 1.0) {
        printf("ERROR: INVALID_INPUT.\n");
        return INVALID_INPUT;
    } else if (str_to_double(argv[1], &epsilon) == -3) {
        printf("ERROR: PROBLEM_EPS.\n");
        return PROBLEM_EPS;
    }

    printf("e:\nlim: %lf, row: %lf, equal: %lf\n", limit_e(epsilon), row_e(epsilon), e_equation(epsilon));
    printf("\npi:\nlim: %lf, row: %lf, equal: %lf\n", limit_pi(epsilon), row_pi(epsilon), pi_equation(epsilon));
    printf("\nln2:\nlim: %lf, row: %lf, equal: %lf\n", limit_ln2(epsilon), row_ln2(epsilon), ln2_equation(epsilon));
    printf("\nsqrt2:\nlim: %lf, row: %lf, equal: %lf\n", limit_sqrt2(epsilon), row_sqrt2(epsilon),
           sqrt2_equation(epsilon));
    printf("\nlambda:\nlim: %lf, row: %lf, equal: %lf\n", limit_lambda(epsilon), row_lambda(epsilon),
           lambda_equation(epsilon));

    return OK;
}

