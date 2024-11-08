#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#define FREE_AND_NULL(x) {\
free((x)); \
(x) = NULL; \
}


typedef enum {
    OK = 0,
    INVALID_INPUT = -1,
    INVALID_FILES = -3,
    MEMORY_ERROR = -4,
    NOT_EXIST_FILE = -5,
    NULL_PTR = -6,
    INVALID_FLAG = -8,
    NOT_FOUNDED_STUDENT = -7,
} state;

typedef struct Student {
    unsigned int id;
    char *name;
    char *surname;
    char *group;
    unsigned char *marks;
    double arithmetic_mean;
} Student;


state simulate_files(char *path_file1, char *path_file2);

state add_students(Student **out_data, FILE *input_file, int *size, double *arithmetic_mean_all);

state check_input_data(Student data);

state find_id(Student *stud_array, int size, unsigned int user_id);

state find_N_SN_G(Student *stud_array, int size, char *promt, char what_searching);

void print_student_in_file(Student student, FILE **output);

void print_student(Student student);

void free_all(Student *stud_array, int size);

int comparator_id(const void *a, const void *b);

int comparator_name(const void *a, const void *b);

int comparator_surname(const void *a, const void *b);

int comparator_group(const void *a, const void *b);

void print_menu();

int main(int argc, char *argv[]) {
    int size = 0;
    char *input_path;
    char *output_path;
    Student *stud_array;
    char user_answer;
    double arithmetic_mean_all;


    if (argc != 2 && argc != 3) {
        printf("ERROR: INVALID_INPUT.\n");
        return INVALID_INPUT;
    }

    if (argc == 2 && (argv[1] == NULL) ||
        argc == 3 && (argv[1] == NULL || argv[2] == NULL)) {
        printf("ERROR: INVALID_INPUT.\n");
        return INVALID_INPUT;
    }

    input_path = argv[1];
    output_path = argv[2];

    if (simulate_files(input_path, output_path) == INVALID_FILES) {
        printf("ERROR: INVALID_FILE. The input file is equal to the output file.\n");
        return INVALID_FILES;
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


    // запись из файла в массив студентов
    state result = add_students(&stud_array, input_file, &size, &arithmetic_mean_all);
    if (result == INVALID_INPUT) {
        fclose(input_file);
        fclose(output_file);
        free_all(stud_array, size);
        printf("ERROR: INVALID_INPUT.\n");
        return INVALID_INPUT;
    } else if (result == MEMORY_ERROR) {
        fclose(input_file);
        fclose(output_file);
        free_all(stud_array, size);
        printf("ERROR: MEMORY_ERROR.\n");
        return MEMORY_ERROR;
    }

    print_menu();
    while ((user_answer = (char) getchar()) != '0') {
        while (user_answer == '\n') {
            user_answer = (char) getchar();
        }
        switch (user_answer) {
            case '1' : {
                unsigned int user_id;
                printf("Enter ID:\t");

                if (scanf("%u", &user_id) != 1) {
                    fclose(input_file);
                    fclose(output_file);
                    free_all(stud_array, size);
                    printf("ERROR: INVALID_INPUT.\n");
                    return INVALID_INPUT;
                }

                state flag_id = find_id(stud_array, size, user_id);
                if (flag_id == NULL_PTR) {
                    printf("ERROR: NULL_PTR.\n");
                    free_all(stud_array, size);
                    fclose(input_file);
                    fclose(output_file);
                    return NULL_PTR;
                }
                if (flag_id == NOT_FOUNDED_STUDENT) {
                    printf("Students with such parameters were found.\n");
                }
                break;
            }
            case '2': {
                char user_q;
                char *need_line;
                state flag_name;

                printf("What are we looking for:\n");
                printf("Name - 'n'\n");
                printf("Surname - 's'\n");
                printf("Group - 'g'\n");

                while ((user_q = (char) getchar()) == '\n') {
                }


                if (user_q != 'n' && user_q != 's' && user_q != 'g') {
                    fclose(input_file);
                    fclose(output_file);
                    free_all(stud_array, size);
                    printf("ERROR: INVALID_INPUT.\n");
                    return INVALID_INPUT;
                }

                need_line = (char *) malloc(sizeof(char));
                if (need_line == NULL) {
                    fclose(input_file);
                    fclose(output_file);
                    free_all(stud_array, size);
                    printf("ERROR: MEMORY_ERROR.\n");
                    return MEMORY_ERROR;
                }

                printf("Input search-word:\n");
                if (scanf("%s", need_line) != 1) {
                    fclose(input_file);
                    fclose(output_file);
                    free(need_line);
                    free_all(stud_array, size);
                    printf("ERROR: INVALID_INPUT.\n");
                    return INVALID_INPUT;
                }

                flag_name = find_N_SN_G(stud_array, size, need_line, user_q);

                if (flag_name == NULL_PTR) {
                    fclose(input_file);
                    fclose(output_file);
                    free(need_line);
                    free_all(stud_array, size);
                    printf("ERROR: NULL_PTR.\n");
                    return NULL_PTR;
                }
                if (flag_name == NOT_FOUNDED_STUDENT) {
                    printf("Students with such parameters were found.\n");
                }
                free(need_line);
                break;
            }
            case '3': {
                qsort(stud_array, size, sizeof(Student), comparator_id);
                printf("Massive was sorted by ID.\n");
                break;
            }
            case '4': {
                qsort(stud_array, size, sizeof(Student), comparator_name);
                printf("Massive was sorted by name.\n");
                break;
            }
            case '5': {
                qsort(stud_array, size, sizeof(Student), comparator_surname);
                printf("Massive was sorted by surname.\n");
                break;
            }
            case '6': {
                qsort(stud_array, size, sizeof(Student), comparator_group);
                printf("Massive was sorted by group.\n");
                break;
            }
            case '7': {
                for (int i = 0; i < size; ++i) {
                    if (stud_array[i].arithmetic_mean > arithmetic_mean_all) {
                        print_student_in_file(stud_array[i], &output_file);
                    }
                }
                printf("The array was successfully analyzed and written "
                       "to the output file.\n");
                break;
            }
            case '8': {
                for (int i = 0; i < size; ++i) {
                    print_student(stud_array[i]);
                }
                break;
            }
            case '0' : {
                fclose(input_file);
                fclose(output_file);
                free_all(stud_array, size);
                return OK;
            }
            default: {
                printf("ERROR: INVALID_FLAG.\n");
                fclose(input_file);
                fclose(output_file);
                free_all(stud_array, size);
                return INVALID_FLAG;
            }
        }
        print_menu();
    }

    fclose(input_file);
    fclose(output_file);
    free_all(stud_array, size);
    return OK;
}


void print_menu() {
    printf("============================================\n");
    printf("1) Search ID.\n");
    printf("2) Search Name \\ Surname \\ Group.\n");
    printf("3) Sorted by ID.\n");
    printf("4) Sorted by Name.\n");
    printf("5) Sorted by Surname.\n");
    printf("6) Sorted by Group.\n");
    printf("7) Find max arithmetic_mean.\n");
    printf("8) Print massive.\n");
    printf("0) Exit. \n");
    printf("============================================\n");
}

state simulate_files(char *path_file1, char *path_file2) {
    char real_path1[BUFSIZ];
    char real_path2[BUFSIZ];

    if (!realpath(path_file1, real_path1) || !realpath(path_file2, real_path2))
        return INVALID_INPUT;

    if (strcmp(real_path1, real_path2) == 0) {
        return INVALID_FILES;
    }

    return OK;
}

void print_student(Student student) {
    printf("---------------------------------------------------------------------\n");
    printf("ID: %u\tName: %s\tSurname: %s\tGroup: %s\tMarks: %c %c %c %c %c\n",
           student.id,
           student.name,
           student.surname,
           student.group,
           student.marks[0],
           student.marks[1],
           student.marks[2],
           student.marks[3],
           student.marks[4]);
    printf("---------------------------------------------------------------------\n");
}

void print_student_in_file(Student student, FILE **output) {
    fprintf(*output, "---------------------------------------------------------------------\n");
    fprintf(*output, "ID: %u\tName: %s\tSurname: %s\tGroup: %s\tMarks: %c %c %c %c %c\n",
            student.id,
            student.name,
            student.surname,
            student.group,
            student.marks[0],
            student.marks[1],
            student.marks[2],
            student.marks[3],
            student.marks[4]);
    fprintf(*output, "---------------------------------------------------------------------\n");
}


state find_id(Student *stud_array, int size, unsigned int user_id) {
    long long int counter = 0;
    if (stud_array == NULL) {
        return NULL_PTR;
    }
    while (counter != size) {
        if (stud_array[counter].id == user_id) {
            print_student(stud_array[counter]);
            return OK;
        }
        ++counter;
    }
    return NOT_FOUNDED_STUDENT;
}

state find_N_SN_G(Student *stud_array, int size, char *promt, char what_searching) {
    long long int counter = 0;
    bool flag = false;

    if (stud_array == NULL || promt == NULL) {
        return NULL_PTR;
    }

    while (counter != size) {
        if (what_searching == 'n') {
            if (strcmp(stud_array[counter].name, promt) == 0) {
                print_student(stud_array[counter]);
                flag = true;
            }
        } else if (what_searching == 's') {
            if (strcmp(stud_array[counter].surname, promt) == 0) {
                print_student(stud_array[counter]);
                flag = true;
            }
        } else if (what_searching == 'g') {
            if (strcmp(stud_array[counter].group, promt) == 0) {
                print_student(stud_array[counter]);
                flag = true;
            }
        }
        ++counter;
    }
    if (!flag) {
        return NOT_FOUNDED_STUDENT;
    }
    return OK;
}


state add_students(Student **out_data, FILE *input_file, int *size, double *arithmetic_mean_all) {
    int args_counter;
    int capacity = 2;
    double arif_mean = 0.0;

    if (input_file == NULL || *out_data == NULL) {
        return INVALID_INPUT;
    }

    Student *data = (Student *) malloc(sizeof(Student) * capacity);
    if (data == NULL) {
        return MEMORY_ERROR;
    }

    data->name = (char *) malloc(sizeof(char) * 52);
    data->surname = (char *) malloc(sizeof(char) * 52);
    data->group = (char *) malloc(sizeof(char) * 52);
    data->marks = (unsigned char *) malloc(sizeof(unsigned char) * 6);
    if (data->name == NULL || data->surname == NULL || data->marks == NULL || data->group == NULL) {
        FREE_AND_NULL(data->name)
        FREE_AND_NULL(data->surname)
        FREE_AND_NULL(data->group)
        FREE_AND_NULL(data->marks)
        FREE_AND_NULL(data)
        return MEMORY_ERROR;
    }

    while (!feof(input_file)) {
        while ((args_counter = fscanf(input_file, "%u%s%s%s%s\r\n",
                                      &(data)[*size].id,
                                      data[*size].name,
                                      data[*size].surname,
                                      data[*size].group,
                                      data[*size].marks)) == 5) {

            if (check_input_data((data)[*size]) == INVALID_INPUT) {
                for (int i = 0; i < *size; ++i) {
                    FREE_AND_NULL(data[i].name)
                    FREE_AND_NULL(data[i].surname)
                    FREE_AND_NULL(data[i].group)
                    FREE_AND_NULL(data[i].marks)
                }
                FREE_AND_NULL(data)
                return INVALID_INPUT;
            }
            arif_mean = 0.0;
            for (int i = 0; i < 5; ++i) {
                arif_mean += (data[*size].marks[0] - '0');
            }
            data[*size].arithmetic_mean = (arif_mean / 5);
            *arithmetic_mean_all += data[*size].arithmetic_mean;

            (*size)++;
            if (*size == capacity) {
                capacity *= 2;
                Student *buffer = (Student *) realloc(data, sizeof(Student) * capacity);
                if (buffer == NULL) {
                    for (int i = 0; i < *size; ++i) {
                        FREE_AND_NULL(data[i].name)
                        FREE_AND_NULL(data[i].surname)
                        FREE_AND_NULL(data[i].group)
                        FREE_AND_NULL(data[i].marks)
                    }
                    FREE_AND_NULL(data)
                    return MEMORY_ERROR;
                }
                data = buffer;
            }

            data[*size].name = (char *) malloc(sizeof(char) * 52);
            data[*size].surname = (char *) malloc(sizeof(char) * 52);
            data[*size].group = (char *) malloc(sizeof(char) * 52);
            data[*size].marks = (unsigned char *) malloc(sizeof(unsigned char) * 6);
            if (data[*size].name == NULL
                || data[*size].surname == NULL
                || data[*size].marks == NULL
                || data[*size].group == NULL) {
                for (int i = 0; i < *size; ++i) {
                    FREE_AND_NULL(data[i].name)
                    FREE_AND_NULL(data[i].surname)
                    FREE_AND_NULL(data[i].group)
                    FREE_AND_NULL(data[i].marks)
                }
                FREE_AND_NULL(data)
                return MEMORY_ERROR;
            }
        }
        if (*arithmetic_mean_all != 0.0) {
            *arithmetic_mean_all /= *size;
        }
    }

    if (args_counter > 0 && args_counter < 5) {
        for (int i = 0; i < *size; ++i) {
            FREE_AND_NULL(data[i].name)
            FREE_AND_NULL(data[i].surname)
            FREE_AND_NULL(data[i].group)
            FREE_AND_NULL(data[i].marks)
        }
        FREE_AND_NULL(data)
        return INVALID_INPUT;
    }

    *out_data = data;
    return OK;
}


state check_input_data(Student data) {
    unsigned long long name_len = strlen(data.name);
    unsigned long long surname_len = strlen(data.surname);

    for (int i = 0; i < name_len; ++i) {
        if (!isalpha(data.name[i])) {
            return INVALID_INPUT;
        }
    }

    for (int i = 0; i < surname_len; ++i) {
        if (!isalpha(data.surname[i])) {
            return INVALID_INPUT;
        }
    }

    for (int i = 0; i < 5; ++i) {
        if (!isdigit(data.marks[i]) || !(data.marks[i] >= '2' && data.marks[i] <= '5')) {
            return INVALID_INPUT;
        }
    }
    return OK;
}


int comparator_id(const void *a, const void *b) {
    const Student *stud1 = (Student *) a;
    const Student *stud2 = (Student *) b;
    return stud1->id - stud2->id;
}

int comparator_name(const void *a, const void *b) {
    const Student *stud1 = (Student *) a;
    const Student *stud2 = (Student *) b;
    return strcmp(stud1->name, stud2->name);
}

int comparator_surname(const void *a, const void *b) {
    const Student *stud1 = (Student *) a;
    const Student *stud2 = (Student *) b;
    return strcmp(stud1->surname, stud2->surname);
}

int comparator_group(const void *a, const void *b) {
    const Student *stud1 = (Student *) a;
    const Student *stud2 = (Student *) b;
    return strcmp(stud1->group, stud2->group);
}


void free_all(Student *stud_array, int size) {
    for (int i = 0; i < size; ++i) {
        FREE_AND_NULL(stud_array[i].name)
        FREE_AND_NULL(stud_array[i].surname)
        FREE_AND_NULL(stud_array[i].group)
        FREE_AND_NULL(stud_array[i].marks)
    }
    FREE_AND_NULL(stud_array)
}