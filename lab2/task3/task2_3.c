#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define SUB_STR "a"

typedef enum State {
    OK,
    INVALID_INPUT,
    INVALID_FILE,
    MEMORY_ERROR,
} state;

typedef struct FoundSubstr {
    char *file_name;
    int n_line;
    int n_char;
    struct found_substr *next;
} found_substr;

int artificial_strlen(const char *str);

state add_new_found_substr(found_substr **head, const char *file_name, int len_filename, int n_line, int n_char);

state search_str_in_files(char *substring, found_substr **result, ...);

int main() {
    found_substr *result, *current;
    char not_found_flag = 1;

    switch (search_str_in_files(SUB_STR, &result, "input.txt", "input2.txt", NULL)) {
        case INVALID_FILE:
            printf("ERROR: INVALID_FILE. Some files could not be opened\n");
            return INVALID_FILE;
        case MEMORY_ERROR:
            printf("ERROR: MEMORY_ERROR.\n");
            return MEMORY_ERROR;
        case INVALID_INPUT:
            printf("ERROR: INVALID_INPUT.\n");
            return INVALID_INPUT;
        default:
            current = result;
            while (current) {
                found_substr *next = current->next;
                not_found_flag = 0;
                printf("File: %s, line: %d, char: %d;\n", current->file_name, current->n_line, current->n_char);
                free(current->file_name);
                free(current);
                current = next;
            }

            if (not_found_flag) {
                printf("Substring not found\n");
            }
    }

    return OK;
}

int artificial_strlen(const char *str) {
    const char *end = str;
    while (*end++);

    return end - str - 1;
}

state add_new_found_substr(found_substr **head, const char *file_name, int len_filename, int n_line, int n_char) {
    if (!head || !file_name)
        return INVALID_INPUT;

    int i;
    found_substr *new_elem = (found_substr *) malloc(sizeof(found_substr));
    if (!new_elem)
        return MEMORY_ERROR;

    new_elem->file_name = (char *) malloc((len_filename + 1) * sizeof(char));
    if (!new_elem->file_name) {
        free(new_elem);
        return MEMORY_ERROR;
    }

    for (i = 0; i <= len_filename; i++) {
        new_elem->file_name[i] = file_name[i];
    }

    new_elem->n_char = n_char;
    new_elem->n_line = n_line;
    new_elem->next = NULL;

    if (!(*head)) {
        *head = new_elem;
    } else {
        found_substr *current = (*head);
        while (current->next) {
            current = current->next;
        }
        current->next = new_elem;
    }
    return OK;
}

state search_str_in_files(char *substring, found_substr **result, ...) {
    if (!substring || !result) {
        return INVALID_INPUT;
    }

    int len_substr = artificial_strlen(substring);
    if (len_substr == 0) {
        return INVALID_INPUT;
    }

    va_list args;
    va_start(args, result);

    *result = NULL;

    char *file_name;
    while ((file_name = va_arg(args, char *)) != NULL) {
        FILE *file = fopen(file_name, "r");
        if (!file) {
            va_end(args);
            return INVALID_FILE;
        }

        int n_line = 1, n_char = 0, idx_substr = 0;
        char c;
        int start_line = 0, start_char = 0;

        while ((c = getc(file)) != EOF) {
            n_char++;

            if (c == substring[idx_substr]) {
                if (idx_substr == 0) {
                    start_line = n_line;
                    start_char = n_char;
                }
                idx_substr++;

                if (idx_substr == len_substr) {
                    if (add_new_found_substr(result,
                                             file_name,
                                             artificial_strlen(file_name),
                                             start_line,
                                             start_char) == MEMORY_ERROR) {
                        fclose(file);
                        va_end(args);
                        return MEMORY_ERROR;
                    }
                    idx_substr--; // Для перекрывающихся подстрок
                    start_char++;  // Сдвигаем стартовый индекс для следующего совпадения
                }
            } else {
                if (idx_substr > 0) {
                    idx_substr = (substring[0] == c) ? 1 : 0;
                    start_char = n_char - idx_substr + 1; // Корректируем стартовый индекс
                    start_line = n_line;
                }
            }

            if (c == '\n') {
                n_line++;
                n_char = 0;
            }
        }

        fclose(file);
    }

    va_end(args);
    return OK;
}
