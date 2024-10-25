#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>


typedef enum STATE_ERRORS {
    OK,
    INVALID_INPUT = -1,
    INVALID_FLAG = -2,
    INVALID_NUMBER = -3,
    ERROR_OVERFLOW = -4,
    ZERO_NUMBER = -5,
    MEMORY_ERROR = -6,
    NEGATIVE_NUMBER = -7,
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


long long int strlen_GG(const char *str) {
    long long int i = 0, counter = 0;

    while (str[i] != '\0') {
        if (counter + 1 > LLONG_MAX) {
            return ERROR_OVERFLOW;
        }
        counter++;
        i++;
    }
    return counter;
}

long long int reverse_GG(char *str, char *new_str) {
    long long int i, len_str = strlen_GG(str);
    char *p_new_str = new_str;
    if (len_str == ERROR_OVERFLOW) {
        return ERROR_OVERFLOW;
    }

    for (i = len_str; i >= 0; --i) {
        if (str[i] == '\0') {
            continue;
        }
        *(p_new_str++) = str[i];

    }
    *p_new_str = '\0';
    return OK;
}


long long int lower_odd_pos(char *str, char *new_str) {
    long long int i, len_str = strlen_GG(str);
    char *p_new_str = new_str;
    if (len_str == ERROR_OVERFLOW) {
        return ERROR_OVERFLOW;
    }

    for (i = 0; i < len_str; ++i) {
        if (i % 2 != 0 && (str[i] >= 'a' && str[i] <= 'z')) {
            *(p_new_str++) = (char) (str[i] - 32);
        } else {
            *(p_new_str++) = str[i];
        }
    }
    *p_new_str = '\0';
    return OK;
}

long long int transmuted_str(char *str, char *new_str) {
    long long int i, len_str = strlen_GG(str);
    char *p_new_str = new_str;

    if (len_str == ERROR_OVERFLOW) {
        return ERROR_OVERFLOW;
    }

    for (i = 0; i < len_str; ++i) {
        if (isdigit(str[i])) {
            *(p_new_str++) = str[i];
        }
    }
    for (i = 0; i < len_str; ++i) {
        if (isalpha(str[i])) {
            *(p_new_str++) = str[i];
        }
    }
    for (i = 0; i < len_str; ++i) {
        if (!isalnum(str[i])) {
            *(p_new_str++) = str[i];
        }
    }

    *p_new_str = '\0';
    return OK;
}

int concatenation(char *str_c, const char *new_str, size_t *buf) {
    if (str_c == NULL) {
        return MEMORY_ERROR;
    }
    long long int len_str_c = strlen_GG(str_c);
    long long int len_new_str = strlen_GG(new_str);

    if (len_new_str == ERROR_OVERFLOW || len_str_c == ERROR_OVERFLOW) {
        return ERROR_OVERFLOW;
    }

    if (*buf < len_str_c + len_new_str + 1) {
        *buf *= 2;
        char *new = (char *) realloc(str_c, *buf);
        if (new == NULL) {
            return MEMORY_ERROR;
        }
        str_c = new;
        free(new);
    }
    for (int i = 0; i < len_new_str; ++i) {
        str_c[len_str_c] = new_str[i];
        len_str_c++;
    }
    str_c[len_str_c] = '\0';
    return OK;
}

int flag_C(char *str_c, char *argv[], long long int argc, size_t buf) {
    long long int random;
    int result, count = 0;
    int flags[argc];

    for (int i = 0; i < argc; ++i) {
        flags[i] = 0;
    }


    while (count != argc) {
        random = 3 + rand() % argc;

        if (flags[random - 3] != 1) {
            result = concatenation(str_c, argv[random], &buf);

            if (result == ERROR_OVERFLOW) {
                return ERROR_OVERFLOW;
            }
            if (result == MEMORY_ERROR) {
                return MEMORY_ERROR;
            }
            flags[random - 3] = 1;
            count++;
        }
    }
    return OK;
}


int main(int argc, char *argv[]) {
    long long int result;
    size_t buf = BUFSIZ;
    if (argc < 3) {
        printf("ERROR: INVALID_INPUT.\n");
        return INVALID_INPUT;
    }

    if (argv[1][0] != '-' && argv[1][2] == '\0') {
        printf("ERROR: INVALID_FLAG.\n");
        return INVALID_FLAG;
    }


    switch (argv[1][1]) {
        case 'l':
            if (argc != 3) {
                printf("ERROR: INVALID_INPUT.\n");
                return INVALID_INPUT;
            }

            result = strlen_GG(argv[2]);
            if (result == ERROR_OVERFLOW) {
                printf("ERROR: ERROR_OVERFLOW.\n");
                return ERROR_OVERFLOW;
            }
            printf("Length of string: %lld\n", result);

            break;

        case 'r':
            if (argc != 3) {
                printf("ERROR: INVALID_INPUT.\n");
                return INVALID_INPUT;
            }
            long long int len_str = strlen_GG(argv[2]);
            if (len_str == ERROR_OVERFLOW) {
                printf("ERROR: ERROR_OVERFLOW.\n");
                return ERROR_OVERFLOW;
            }
            char *reversed_str_r = (char *) malloc(sizeof(char) * (len_str + 1));
            result = reverse_GG(argv[2], reversed_str_r);
            if (result == ERROR_OVERFLOW) {
                printf("ERROR: ERROR_OVERFLOW.\n");
                free(reversed_str_r);
                return ERROR_OVERFLOW;
            }
            printf("Reversed string: %s\n", reversed_str_r);
            free(reversed_str_r);
            break;

        case 'u':
            if (argc != 3) {
                printf("ERROR: INVALID_INPUT.\n");
                return INVALID_INPUT;
            }
            long long int len_str_u = strlen_GG(argv[2]);
            if (len_str_u == ERROR_OVERFLOW) {
                printf("ERROR: ERROR_OVERFLOW.\n");
                return ERROR_OVERFLOW;
            }

            char *str_u = (char *) malloc(sizeof(char) * (len_str_u + 1));
            result = lower_odd_pos(argv[2], str_u);
            if (result == ERROR_OVERFLOW) {
                printf("ERROR: ERROR_OVERFLOW.\n");
                free(str_u);
                return ERROR_OVERFLOW;
            }
            printf("The letters in odd places have been converted to uppercase: %s\n", str_u);
            free(str_u);
            break;

        case 'n': {
            if (argc != 3) {
                printf("ERROR: INVALID_INPUT.\n");
                return INVALID_INPUT;
            }

            long long int len_str_n = strlen_GG(argv[2]);
            if (len_str_n == ERROR_OVERFLOW) {
                printf("ERROR: ERROR_OVERFLOW.\n");
                return ERROR_OVERFLOW;
            }

            char *transmuted_str_n = (char *) malloc(sizeof(char) * (len_str_n + 1));
            result = transmuted_str(argv[2], transmuted_str_n);

            if (result == ERROR_OVERFLOW) {
                printf("ERROR: ERROR_OVERFLOW.\n");
                free(transmuted_str_n);
                return ERROR_OVERFLOW;
            }
            printf("Transmuted successful: %s\n", transmuted_str_n);
            free(transmuted_str_n);
        }
            break;


        case 'c':
            long long int seed;
            if (str_to_int(argv[2], &seed) == INVALID_NUMBER) {
                printf("ERROR: INVALID_NUMBER.\n");
                return INVALID_NUMBER;
            }

            srand(seed);
            char *str_c = (char *) malloc(buf * sizeof(char));
            str_c[0] = '\0';
            result = flag_C(str_c, argv, argc - 3, buf);

            if (result == ERROR_OVERFLOW) {
                printf("ERROR: ERROR_OVERFLOW.\n");
                free(str_c);
                return ERROR_OVERFLOW;
            }
            printf("New str: %s\n", str_c);
            free(str_c);
            break;
        default:
            printf("ERROR: INVALID_FLAG.\n");
            return INVALID_FLAG;
    }
}
