#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <windows.h>
#include <limits.h>


typedef enum STATE_ERRORS {
    OK,
    INVALID_INPUT = -1,
    INVALID_NUMBER = -2,
    NOT_EXIST_FILE = -7,
    INVALID_FILE = -8,
    MEMORY_ERROR = -10,
    INVALID_WORD = -11,
} STATE_ERRORS;


int str_to_llint(const char *str, long long int *x, int base) {
    char *end_p;
    long long num = strtoll(str, &end_p, base);

    if (num >= LLONG_MAX || num <= LLONG_MIN) {
        return INVALID_NUMBER;
    }
    if (end_p == str && *x == 0) {
        return INVALID_NUMBER;
    }
    if (*end_p != '\0') {
        return INVALID_NUMBER;
    }

    *x = num;
    return OK;
}

int checker_smule_files(char *file1_path, char *file2_path) {
    char real_path1[MAX_PATH];
    char real_path2[MAX_PATH];

    if (GetFullPathName(file1_path, MAX_PATH, real_path1, NULL) == 0) {
        return INVALID_FILE;
    }

    if (GetFullPathName(file2_path, MAX_PATH, real_path2, NULL) == 0) {
        return INVALID_FILE;
    }

    if (strcmp(real_path1, real_path2) == 0) {
        return INVALID_FILE;
    }

    return OK;
}


int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("ERROR: INVALID_INPUT.\n");
        return INVALID_INPUT;
    }

    char *input_path = argv[1];
    char *output_path = argv[2];

    if (checker_smule_files(input_path, output_path) == INVALID_FILE) {
        printf("ERROR: INVALID_FILE. The input file is equal to the output file");
        return INVALID_FILE;
    }

    // Открываем input файл
    FILE *input_file = fopen(input_path, "r");
    if (input_file == NULL) {
        printf("ERROR: NOT_EXIST_FILE. The file with this name does not exist.\n");
        return NOT_EXIST_FILE;
    }

    // Открываем output файл
    FILE *output_file = fopen(output_path, "w");
    if (output_file == NULL) {
        fclose(input_file);
        printf("ERROR: NOT_EXIST_FILE. The file with this name does not exist.\n");
        return NOT_EXIST_FILE;
    }

    int symbol, result, counter_n = 0, max_base = -1;

    char *number = (char *) calloc(65, sizeof(char));

    if (number == NULL) {
        fclose(input_file);
        fclose(output_file);
        return MEMORY_ERROR;
    }

    while (!feof(input_file)) {
        symbol = fgetc(input_file);

        if (symbol == EOF) {
            continue;
        }

        if (!isspace(symbol)) {
            if (symbol == '0' && counter_n == 0) {
                continue;
            }

            number[counter_n] = (char) symbol;
            counter_n++;

            if (isdigit(symbol)) {
                if (symbol - 47 > max_base) {
                    max_base = symbol - 47;
                }
            } else if (isalpha(symbol)) {
                if (symbol - 86 > max_base) {
                    max_base = symbol - 86;
                }
            } else {
                printf("ERROR: INVALID_WORD.\n");

                free(number);
                fclose(input_file);
                fclose(output_file);
                return INVALID_WORD;
            }
            number[counter_n] = '\0';

        } else {
            if (counter_n != 0 && max_base != -1) {
                long long int x = 0;
                result = str_to_llint(number, &x, max_base);

                if (result == INVALID_NUMBER) {
                    printf("ERROR: INVALID_NUMBER.\n");

                    free(number);
                    fclose(input_file);
                    fclose(output_file);
                    return INVALID_NUMBER;
                } else {
                    fprintf(output_file, "%s %d %lld\n", number, max_base, x);
                }
            }
            counter_n = 0;
            max_base = -1;
        }
    }

    // Обработка последнего числа, если оно есть
    if (counter_n != 0 && max_base != -1) {
        long long int x = 0;
        result = str_to_llint(number, &x, max_base);

        if (result == INVALID_NUMBER) {
            printf("ERROR: INVALID_NUMBER.\n");
            free(number);
            fclose(input_file);
            fclose(output_file);
            return INVALID_NUMBER;
        } else {
            fprintf(output_file, "%s %d %lld\n", number, max_base, x);
        }
    }

    printf("The incoming file was successfully analyzed, and the results were recorded in the output file.\n");
    free(number);
    fclose(input_file);
    fclose(output_file);
    return OK;
}
