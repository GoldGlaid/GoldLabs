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
    INVALID_FLAG = -2,
    NOT_EXIST_FILE = -7,
    INVALID_FILE = -8,
    MEMORY_ERROR = -10,
} STATE_ERRORS;


int checker_smule_files(char *file1_path, char *file2_path) {
    char real_path1[MAX_PATH];
    char real_path2[MAX_PATH];

    if (GetFullPathName(file1_path, MAX_PATH, real_path1, NULL) == 0) {
        return INVALID_FILE;
    }

    if (GetFullPathName(file2_path, MAX_PATH, real_path2, NULL) == 0) {
        return INVALID_FILE;
    }

    if (strcmp(real_path1, real_path2) == 0)
    {
        return INVALID_FILE;
    }

    return OK;
}


// even - четный, odd - нечетный
int flag_R(FILE *input_file1, FILE *input_file2, FILE *output_file) {
    int flag = 1, start_flag = 1;
    int symbol;
    while (!feof(input_file1) && !feof(input_file2)) {
        if (flag % 2 != 0) {
            bool word1 = false;
            symbol = fgetc(input_file1);

            while (isspace(symbol)) {
                symbol = fgetc(input_file1);
            }

            while (symbol != EOF && symbol != '\n' && symbol != ' ' && symbol != '\t') {
                if (!word1) {
                    if (!start_flag) {
                        fputc(' ', output_file);
                        word1 = true;
                    }

                }
                fputc(symbol, output_file);
                symbol = fgetc(input_file1);
            }
            ++flag;
        } else if (flag % 2 == 0) {
            bool word2 = false;
            symbol = fgetc(input_file2);
            start_flag = 0;

            while (isspace(symbol)) {
                symbol = fgetc(input_file2);
            }

            while (symbol != EOF && symbol != '\n' && symbol != ' ' && symbol != '\t') {
                if (!word2) {
                    fputc(' ', output_file);
                    word2 = true;
                }
                fputc(symbol, output_file);
                symbol = fgetc(input_file2);
            }
            ++flag;
        }
    }

    while (!feof(input_file1)) {
        bool word = false;
        while ((symbol = fgetc(input_file1)) != EOF) {
            if (!isspace(symbol)) {
                if (!word) {
                    fputc(' ', output_file);
                    word = true;
                }
                fputc(symbol, output_file);
            } else {
                word = false;
            }
        }
    }

    while (!feof(input_file2)) {
        bool word = false;
        while ((symbol = fgetc(input_file2)) != EOF) {
            if (!isspace(symbol)) {
                if (!word) {
                    fputc(' ', output_file);
                    word = true;
                }
                fputc(symbol, output_file);
            } else {
                word = false;
            }
        }
    }

    return OK;
}


int str_to_others(int number, int system, FILE *file) {
    if (system < 2 || system > 36) {
        return MEMORY_ERROR;
    }

    if (number == 0) {
        fprintf(file, "0");
        return OK;
    }

    int count = 0;
    char *array = malloc(1);

    if (array == NULL) {
        return MEMORY_ERROR;
    }
    while (number > 0) {
        // +1 для нового символа, +1 для конечного нуля
        char *new_check = realloc(array, (count + 2) * sizeof(char));
        if (new_check == NULL) {
            free(array);
            return MEMORY_ERROR;
        }
        array = new_check;

        if (number % system <= 9) {
            array[count] = (char)(number % system + '0');
        } else {
            array[count] = (char)((number % system) - 10 + 'a'); // Для систем счисления > 10
        }

        number /= system;
        count++;
    }

    array[count] = '\0';

    // Запись в файл в обратном порядке
    for (int i = count - 1; i >= 0; --i) {
        fprintf(file, "%c", array[i]);
    }

    free(array);
    return OK;
}

int main(int argc, char *argv[]) {
    if (argc < 4 || argc > 5) {
        printf("ERROR: INVALID_INPUT.\n");
        return INVALID_INPUT;
    }

    if (!(argv[1][0] == '-' || argv[1][0] == '/') && argv[1][2] == '\0') {
        printf("ERROR: INVALID_FLAG.\n");
        return INVALID_FLAG;
    }

    char flag = argv[1][1];
    switch (flag) {
        case ('r'):
            if (argc != 5) {
                printf("ERROR: INVALID_INPUT.\n");
                return INVALID_INPUT;
            }

            char *file1_path = argv[2];
            char *file2_path = argv[3];
            char *file_output_path = argv[4];

            // проверка на несовпадения 1 и 2 файлов
            if (checker_smule_files(file1_path, file2_path) == INVALID_FILE) {
                printf("ERROR: INVALID_FILE. The input file is equal to the output file");
                return INVALID_FILE;
            }

            // проверка на несовпадения 1 и 3 файлов
            if (checker_smule_files(file1_path, file_output_path) == INVALID_FILE) {
                printf("ERROR: INVALID_FILE. The input file is equal to the output file");
                return INVALID_FILE;
            }

            // проверка на несовпадения 2 и 3 файлов
            if (checker_smule_files(file2_path, file_output_path) == INVALID_FILE) {
                printf("ERROR: INVALID_FILE. The input file is equal to the output file");
                return INVALID_FILE;
            }

            // Открываем input1 файл
            FILE *input_file1 = fopen(file1_path, "r");
            if (input_file1 == NULL) {
                printf("ERROR: NOT_EXIST_FILE. The file with this name does not exist.\n");
                return NOT_EXIST_FILE;
            }

            // Открываем input2 файл
            FILE *input_file2 = fopen(file2_path, "r");
            if (input_file2 == NULL) {
                fclose(input_file1);
                printf("ERROR: NOT_EXIST_FILE. The file with this name does not exist.\n");
                return NOT_EXIST_FILE;
            }

            // Открываем output файл
            FILE *output_file = fopen(file_output_path, "w");
            if (output_file == NULL) {
                fclose(input_file1);
                fclose(input_file2);
                printf("ERROR: NOT_EXIST_FILE. The file with this name does not exist.\n");
                return NOT_EXIST_FILE;
            }

            flag_R(input_file1, input_file2, output_file);
            printf("All tokens were successfully recorded.\n");

            fclose(input_file1);
            fclose(input_file2);
            fclose(output_file);
            break;
        case ('a'):
            if (argc != 4) {
                printf("ERROR: INVALID_INPUT.\n");
                return INVALID_INPUT;
            }

            char *input_path = argv[2];
            char *output_path = argv[3];

            // проверка на несовпадения 2 и 3 файлов
            if (checker_smule_files(input_path, output_path) == INVALID_FILE) {
                printf("ERROR: INVALID_FILE. The input file is equal to the output file");
                return INVALID_FILE;
            }

            // Открываем input1 файл
            FILE *input = fopen(input_path, "r");
            if (input == NULL) {
                printf("ERROR: NOT_EXIST_FILE. The file with this name does not exist.\n");
                return NOT_EXIST_FILE;
            }

            // Открываем output файл
            FILE *output = fopen(output_path, "w");
            if (output == NULL) {
                fclose(input);
                printf("ERROR: NOT_EXIST_FILE. The file with this name does not exist.\n");
                return NOT_EXIST_FILE;
            }

            int symb, result, counter = 1, space_flag = -1;
            while (!feof(input)) {
                symb = fgetc(input);

                if (symb == EOF) {
                    continue;
                }
                if (!isspace(symb)) {
                    if (counter % 10 == 0) {
                        if (isalpha(symb) && (symb >= 65 && symb <= 90)) {
                            symb += 32;
                        }
                        result = str_to_others(symb, 4, output);
                        if (result == MEMORY_ERROR) {
                            printf("ERROR: MEMORY_ERROR.\n");
                        }
                        space_flag = 0;
                    } else if (counter % 2 == 0) {
                        if (isalpha(symb) && (symb >= 65 && symb <= 90)) {
                            symb += 32;
                        }
                        fputc(symb, output);
                        space_flag = 0;
                    } else if (counter % 5 == 0) {
                        str_to_others(symb, 8, output);
                        space_flag = 0;
                    } else {
                        fputc(symb, output);
                        space_flag = 0;
                    }
                } else {
                    if (space_flag == 0) {
                        ++counter;
                        fputc(' ', output);
                        space_flag = -1;
                    }
                }
            }

            break;
        default:
            printf("ERROR: INVALID_FLAG.\n");
            return INVALID_FLAG;
    }
    return OK;
}
