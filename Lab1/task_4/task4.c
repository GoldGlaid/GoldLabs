#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BUFFER_SIZE 100

typedef enum STATE_ERRORS {
    OK,
    INVALID_INPUT = -1,
    INVALID_FLAG = -2,
    INVALID_NUMBER = -3,
    ERROR_OVERFLOW = -4,
    ZERO_NUMBER = -5,
    NEGATIVE_NUMBER = -6,
    NOT_EXIST_FILE = -7,
} STATE_ERRORS;

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
int str_to_long_double(const char *str, double *result) {
    int dot_flag = 0;

    for (int i = 0; i < strlen(str); ++i) {
        if ((str[i] < '0' || str[i] > '9') && str[i] != '.' && str[i] != '-') {
            return INVALID_NUMBER;
        }
        if (str[i] == '-' && i != 0) {
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
    double num = strtod(str, &end_p);

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

//TODO Обработать если входящий файл == выходящему файлу
int flag_D(char *argv[], bool n) {

    FILE *input_file = fopen(argv[2], "r");
    if (n) {
        FILE *output_file = fopen(argv[3], "r");
    } else {
        char

    }
    if (input_file == NULL) {
        fclose(input_file);
        return NOT_EXIST_FILE;
    }

}

//name -h path
//name -nh path outpath

int main(int argc, char *argv[]) {
    if (argc < 3 || argc > 4) {
        printf("ERROR: INVALID_INPUT.\n");
        return INVALID_INPUT;
    }

    if (!(argv[1][0] == '-' || argv[1][0] == '/') && argv[1][2] == '\0') {
        printf("ERROR: INVALID_FLAG.\n");
        return INVALID_FLAG;
    }


    char flag;
    bool n_flag = false;

// TODO Скорее-всего, нужно засунуть это в отдельную функцию
    if (argv[1][1] == 'n') {
        flag = argv[1][2];
        n_flag = true;
    } else {
        flag = argv[1][1];
    }

    switch (flag) {
        case ('d'):
            int result = flag_D(argv, n_flag);

            if (result == NOT_EXIST_FILE) {
                printf("ERROR: NOT_EXIST_FILE. The file with this name does not exist.\n");
                return NOT_EXIST_FILE;
            }

//            char str[BUFFER_SIZE];
//            while (fgets(str, BUFFER_SIZE, input_file) != NULL) {
//
//            }
            break;

    }

//    FILE *output_file = NULL;

    fclose(input_file);
}
