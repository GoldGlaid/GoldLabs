#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


typedef enum STATE_ERRORS {
    OK,
    INVALID_INPUT = -1,
    INVALID_FLAG = -2,
    NOT_EXIST_FILE = -7,
    INVALID_FILE = -8,
    MEMORY_ERROR = -9,
} STATE_ERRORS;


void remove_numbers(FILE *input, FILE *output) {
    int symbol;
    while ((symbol = fgetc(input)) != EOF) {
        if (symbol >= '0' && symbol <= '9') {
            continue;
        }
        fputc(symbol, output);
    }
}

void count_roman_alphabet(FILE *input, FILE *output) {
    int symbol;
    long long int counter_symbols_RA = 0, counter_rows = 1;

    fprintf(output, "%lld) ", counter_rows);
    while ((symbol = fgetc(input)) != EOF) {
        if (symbol == '\n') {
            fprintf(output, "%lld\n", counter_symbols_RA);
            counter_symbols_RA = 0;

            counter_rows++;
            fprintf(output, "%lld) ", counter_rows);
        } else {
            if ((symbol >= 'A' && symbol <= 'Z') || (symbol >= 'a' && symbol <= 'z')) {
                counter_symbols_RA++;
            }
        }
    }
    fprintf(output, "%lld\n", counter_symbols_RA);
}


void count_specific_symbols(FILE *input, FILE *output) {
    int symbol;
    long long int counter_symbols_RA = 0, counter_rows = 1;

    fprintf(output, "%lld) ", counter_rows);
    while ((symbol = fgetc(input)) != EOF) {
        if (symbol == '\n') {
            counter_symbols_RA++;
            fprintf(output, "%lld\n", counter_symbols_RA);
            counter_symbols_RA = 0;

            counter_rows++;
            fprintf(output, "%lld) ", counter_rows);
        } else {
            if (!((symbol >= 'A' && symbol <= 'Z') ||
                  (symbol >= 'a' && symbol <= 'z') ||
                  (symbol >= '0' && symbol <= '9') || symbol == ' ')) {
                counter_symbols_RA++;
            }
        }
    }
    counter_symbols_RA++;
    fprintf(output, "%lld\n", counter_symbols_RA);
}


void replace_symbols_xcode(FILE *input, FILE *output) {
    int symbol;
    while ((symbol = fgetc(input)) != EOF) {
        if (symbol == '\n' || (symbol >= '0' && symbol <= '9')) {
            fputc(symbol, output);
            continue;
        }
        fprintf(output, "%x", symbol);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3 || argc > 4) {
        printf("ERROR: INVALID_INPUT.\n");
        return INVALID_INPUT;
    }

    if (!(argv[1][0] == '-' || argv[1][0] == '/') && (argv[1][2] == '\0' || argv[1][3] == '\0')) {
        printf("ERROR: INVALID_FLAG.\n");
        return INVALID_FLAG;
    }

    char flag;
    bool n_flag = false;

    if (argv[1][1] == 'n') {
        if (argc == 4) {
            flag = argv[1][2];
            n_flag = true;
        } else {
            printf("ERROR: INVALID_INPUT.\n");
            return INVALID_INPUT;
        }
    } else {
        flag = argv[1][1];
    }

    // Открываем input файл
    FILE *input_file = fopen(argv[2], "r");
    if (input_file == NULL) {
        printf("ERROR: NOT_EXIST_FILE. The file with this name does not exist.\n");
        return NOT_EXIST_FILE;
    }


    FILE *output_file;

    // Открываем output файл (При ошибке закрываем input файл)
    if (n_flag) {
        if (strcmp(argv[2], argv[3]) == 0) {
            fclose(input_file);
            printf("ERROR: INVALID_FILE. The input file is equal to the output file");
            return INVALID_FILE;
        }

        output_file = fopen(argv[3], "w");
        if (output_file == NULL) {
            fclose(input_file);
            printf("ERROR: NOT_EXIST_FILE. The file with this name does not exist.\n");
            return NOT_EXIST_FILE;
        }

    } else {
        // Выделили память - в дальнейшем нужно закрывать
        char *outfile_path = (char *) malloc((strlen(argv[2]) + 6) * sizeof(char));
        if (outfile_path == NULL) {
            fclose(input_file);
            printf("ERROR: MEMORY_ERROR.\n");
            return MEMORY_ERROR;
        }
        // Возможная ошибка связанная с /. Может быть разделитель: 2x Обратный слеш
        char *last_slash = strrchr(argv[2], '\\');
        if (last_slash == NULL) {
            strcat(outfile_path, "out_");
            strcat(outfile_path, argv[2]);
            outfile_path[strlen(argv[2]) + strlen("out_") + 1] = '\0';
        } else {
            char *old_name = last_slash + 1;
            unsigned long long int path_without_name = strlen(argv[2]) - strlen(old_name);

            strncpy(outfile_path, argv[2], path_without_name);
            outfile_path[path_without_name] = '\000';
            strcat(outfile_path, "out_");
            strcat(outfile_path, old_name);
            outfile_path[strlen(argv[2]) + strlen("out_") + 1] = '\0';
        }

        output_file = fopen(outfile_path, "w");
        free(outfile_path);

        if (output_file == NULL) {
            fclose(input_file);
            printf("ERROR: NOT_EXIST_FILE. The file with this name does not exist.\n");
            return NOT_EXIST_FILE;
        }
    }
    switch (flag) {
        case ('d'):
            remove_numbers(input_file, output_file);
            printf("Arabic numerals were excluded.\n"
                   "The output file was successfully generated\n");
            break;
        case ('i'):
            count_roman_alphabet(input_file, output_file);
            printf("The number of letters from the Latin alphabet has been counted.\n"
                   "The output file was successfully generated\n");
            break;
        case ('s'):
            count_specific_symbols(input_file, output_file);
            printf("The number of letters has been counted.\n"
                   "The output file was successfully generated\n");
            break;
        case ('a'):
            replace_symbols_xcode(input_file, output_file);
            printf("The letters were replaced by an ASCII code written in the base 16 numeral system.\n"
                   "The output file was successfully generated\n");
            break;
        default:
            fclose(input_file);
            fclose(output_file);

            printf("ERROR: INVALID_FLAG.\n");
            return INVALID_FLAG;
    }
    fclose(input_file);
    fclose(output_file);
    return OK;
}
