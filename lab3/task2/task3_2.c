#include "functions.h"

int main() {
    vector v_1 = {.data = (double[]) {-1.0, -100.0, -3.0, 10.0},
                  .n = 4
    };
    vector v_2 = {.data = (double[]) {0.0, 4.0, 6.0, 100.},
                  .n = 4
    };

    vector v_3 = {.data = (double[]) {-5.0, 50.0, -2.0, -2.},
            .n = 4
    };

    double matrix_A[] = {52.0, 26.0, 472.0, 55.5,
                         13.0, 16.0, 3.0, 4.0,
                         70, 9, 5, 3,
                         19, 21, 4, 25};
    double p = 11.232;

    size_t count_res = 0;
    vector *result_arr = NULL;

    switch (search_longest_vectors(&result_arr, &count_res, 3, 3,
                                   v_1, v_2, v_3,
                                   inf_norm, NULL,
                                   p_norm, &p,
                                   spectr_norm, &matrix_A)) {
        case MEMORY_ERROR:
            printf("ERROR: MEMORY_ERROR.\n");
            destroy_vector(&result_arr, count_res);
            return MEMORY_ERROR;
        case INPUT_ERROR:
            printf("ERROR: INPUT_ERROR.\n");
            destroy_vector(&result_arr, count_res);
            return MEMORY_ERROR;
        case OVERFLOW_ERROR:
            printf("ERROR: OVERFLOW_ERROR.\n");
            destroy_vector(&result_arr, count_res);
            return OVERFLOW_ERROR;
        case OK:
            for (size_t i = 0; i < count_res - 1; ++i) {
                if (!result_arr[i].data) {
                    putchar('\n');
                    continue;
                }
                printf("( ");
                for (size_t j = 0; j < result_arr[i].n; ++j) {
                    printf("%lf ", result_arr[i].data[j]);
                }
                printf(")\n");
            }
    }

    destroy_vector(&result_arr, count_res);
    return 0;
}