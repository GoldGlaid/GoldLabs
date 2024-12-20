#ifndef GOLDLABS_FUNCTIONS_H
#define GOLDLABS_FUNCTIONS_H

#include "stdio.h"
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>

typedef enum State {
    OK,
    INPUT_ERROR,
    MEMORY_ERROR,
    OVERFLOW_ERROR
} state;

typedef struct Vector {
    double *data;
    size_t n;
} vector;

typedef double (*norm_t)(vector *, void *);

void destroy_vector(vector **arr, size_t size);

double spectr_norm(vector *vec, void *args);

double inf_norm(vector *vec, void *args);

double p_norm(vector *vec, void *args);

state search_longest_vectors(vector **res, size_t *count_res, size_t vectors_cnt, size_t count_norms, ...);

#endif //GOLDLABS_FUNCTIONS_H
