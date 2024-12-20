#ifndef GOLDLABS_FUNCTIONS_H
#define GOLDLABS_FUNCTIONS_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

int str_to_double(char *str, double *number);

double limit_e(double epsilon);

double limit_pi(double epsilon);

double limit_ln2(double epsilon);

double limit_sqrt2(double epsilon);

double limit_lambda(double epsilon);

double row_e(double epsilon);

double row_pi(double epsilon);

double row_ln2(double epsilon);

double row_sqrt2(double epsilon);

double row_lambda(double epsilon);

double e_equation(double epsilon);

double pi_equation(double epsilon);

double sqrt2_equation(double epsilon);

double ln2_equation(double epsilon);

double lambda_equation(double epsilon);

int is_prime(int number);

enum State {
    OK,
    INVALID_INPUT,
    PROBLEM_EPS,
    NULL_POINTER,
};

#endif //GOLDLABS_FUNCTIONS_H
