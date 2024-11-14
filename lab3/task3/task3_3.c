#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <float.h>


#define EPS 1e-10

typedef enum STATE_ERRORS {
    OK,
    INVALID_INPUT = -1,
    NOT_EXIST_FILE = -3,
    INVALID_FILE = -4,
    MEMORY_ERROR = -5,
    INVALID_FLAG = -6,
} error;


typedef struct Employee {
    unsigned int id;
    char name[64];
    char surname[64];
    double salary;
} Employee;


error checker_simulate_files(char *file1_path, char *file2_path);

error add_employees(Employee **out_data, FILE *input_file, int *size);

error check_input_data(Employee massive_employees);

int comparator_a(const void *a, const void *b);

int comparator_d(const void *a, const void *b);


int main(int argc, char *argv[]) {

    int size = 0;
    char *output_path;
    char *input_path;
    char flag;
    Employee *massive_employees;

    if (argc != 4) {
        printf("ERROR: INVALID_INPUT.\n");
        return INVALID_INPUT;
    }

    if (argv[1] == NULL || argv[2] == NULL || argv[3] == NULL) {
        printf("ERROR: INVALID_INPUT.\n");
        return INVALID_INPUT;
    }

    if (!((argv[1][0] == '-' || argv[1][0] == '/') && argv[1][2] == '\0')) {
        printf("ERROR: INVALID_FLAG.\n");
        return INVALID_FLAG;
    }

    input_path = argv[2];
    output_path = argv[3];

    if (checker_simulate_files(input_path, output_path) == INVALID_FILE) {
        printf("ERROR: INVALID_FILE. The input file is equal to the output file.\n");
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

    flag = argv[1][1];
    massive_employees = NULL;

    switch (flag) {
        case 'a': {
            error result = add_employees(&massive_employees, input_file, &size);
            if (result == INVALID_INPUT) {
                fclose(input_file);
                fclose(output_file);
                printf("ERROR: INVALID_INPUT.\n");
                return INVALID_INPUT;
            } else if (result == MEMORY_ERROR) {
                fclose(input_file);
                fclose(output_file);
                printf("ERROR: MEMORY_ERROR.\n");
                return MEMORY_ERROR;
            }

            qsort(massive_employees, size, sizeof(Employee), comparator_a);
            for (int i = 0; i < size; ++i) {
                fprintf(output_file, "%u %s %s %lf\n",
                        massive_employees[i].id,
                        massive_employees[i].name,
                        massive_employees[i].surname,
                        massive_employees[i].salary);
            }

            break;
        }
        case 'd':
            error result = add_employees(&massive_employees, input_file, &size);
            if (result == INVALID_INPUT) {
                fclose(input_file);
                fclose(output_file);
                printf("ERROR: INVALID_INPUT.\n");
                return INVALID_INPUT;
            } else if (result == MEMORY_ERROR) {
                fclose(input_file);
                fclose(output_file);
                printf("ERROR: MEMORY_ERROR.\n");
                return MEMORY_ERROR;
            }

            qsort(massive_employees, size, sizeof(Employee), comparator_d);
            for (int i = 0; i < size; ++i) {
                fprintf(output_file, "%u %s %s %lf\n",
                        massive_employees[i].id,
                        massive_employees[i].name,
                        massive_employees[i].surname,
                        massive_employees[i].salary);
            }
            break;

        default:
            free(massive_employees);
            fclose(input_file);
            fclose(output_file);
            printf("ERROR: INVALID_FLAG.\n");
            return INVALID_FLAG;
    }
    free(massive_employees);
    fclose(input_file);
    fclose(output_file);
    printf("The operation was carried out successfully\n");
    return OK;
}

error checker_simulate_files(char *file1_path, char *file2_path) {
    char real_path1[BUFSIZ];
    char real_path2[BUFSIZ];

    if (!realpath(file1_path, real_path1) || !realpath(file2_path, real_path2))
        return INVALID_INPUT;

    if (strcmp(real_path1, real_path2) == 0) {
        return INVALID_FILE;
    }

    return OK;
}

error add_employees(Employee **out_data, FILE *input_file, int *size) {
    int args_counter, capacity = 2;

    Employee *data = (Employee *) malloc(sizeof(Employee) * capacity);
    if (data == NULL) {
        return MEMORY_ERROR;
    }

    while ((args_counter = fscanf(input_file, "%u%s%s%lf\n",
                                  &(data)[*size].id,
                                  (data)[*size].name,
                                  (data)[*size].surname,
                                  &(data)[*size].salary)) == 4) {

        if (check_input_data((data)[*size]) == INVALID_INPUT) {
            free(data);
            return INVALID_INPUT;
        }

        (*size)++;
        //  Перевыделение памяти
        if (*size == capacity) {
            capacity *= 2;
            Employee *buffer = (Employee *) realloc(data, sizeof(Employee) * capacity);
            if (buffer == NULL) {
                free(data);
                return MEMORY_ERROR;
            }
            data = buffer;
        }
    }

    if (args_counter > 0 && args_counter < 4) {
        free(data);
        return INVALID_INPUT;
    }

    *out_data = data;
    return OK;
}


error check_input_data(Employee massive_employees) {
    unsigned long long len_name = strlen(massive_employees.name);
    unsigned long long len_surname = strlen(massive_employees.surname);

    if (massive_employees.salary < EPS) {
        return INVALID_INPUT;
    }

    for (int i = 0; i < len_name; ++i) {
        if (!isalpha(massive_employees.name[i])) {
            return INVALID_INPUT;
        }
    }

    for (int i = 0; i < len_surname; ++i) {
        if (!isalpha(massive_employees.surname[i])) {
            return INVALID_INPUT;
        }
    }

    return OK;
}

int comparator_a(const void *a, const void *b) {
    const Employee *employe_1 = (Employee *) a;
    const Employee *employe_2 = (Employee *) b;
    int result;
    if (employe_1->salary - employe_2->salary > EPS) {
        return 1;
    }

    if (employe_2->salary - employe_1->salary > EPS) {
        return -1;
    }

    result = strcmp(employe_1->surname, employe_2->surname);
    if (result) {
        return 1;
    }

    result = strcmp(employe_1->name, employe_2->name);
    if (result) {
        return 1;
    }
    return employe_1->id - employe_2->id;
}


int comparator_d(const void *a, const void *b) {
    return comparator_a(a, b) * -1;
}