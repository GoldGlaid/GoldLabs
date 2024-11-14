#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#define FREE_AND_NULL(x) {\
free((x)); \
(x) = NULL; \
}

#define HASHSIZE 128

typedef struct macro {
    char *key;
    char *value;
    struct macro *next;
} macro;

typedef struct hashtable {
    int size;
    struct macro **table;
} hashtable;

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

state simulate_files(char *path_file1, char *path_file2);

state add_defines(macro ***out_data, FILE *input_file);

unsigned int hash(char *key, int table_size);

void insert(struct hashtable *ht, char *key, char *value);

void rehash(struct hashtable *ht);


int main(int argc, char *argv[]) {
    int size = 0;
    char *input_path;
    char *output_path;
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


    printf("%d", hash("MAX_VALUE", 128));
    hashtable ht;

    // запись из файла в массив
    state result = add_defines(&ht.table, input_file);
    if (result == MEMORY_ERROR) {
        fclose(input_file);
        fclose(output_file);
//        free_all(ht.table, size);
        printf("ERROR: MEMORY_ERROR.\n");
        return MEMORY_ERROR;
    }


//    while ((user_answer = (char) getchar()) != '0') {
//        while (user_answer == '\n') {
//            user_answer = (char) getchar();
//        }
//        switch (user_answer) {
//            default: {
//                printf("ERROR: INVALID_FLAG.\n");
//                fclose(input_file);
//                fclose(output_file);
//                return INVALID_FLAG;
//            }
//        }
//    }

    fclose(input_file);
    fclose(output_file);
    return OK;
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


unsigned int hash(char *key, int table_size) {
    unsigned int hash_value = 0;
    while (*key) {
        hash_value = (hash_value * 62 + (*key - '0')) % table_size;
        key++;
    }
    return hash_value;
}


void insert(hashtable *ht, char *key, char *value) {
    unsigned int index = hash(key, ht->size);
    macro *new_macro = (macro *) malloc(sizeof(macro));
    new_macro->key = key;
    new_macro->value = value;
    new_macro->next = ht->table[index];  // добавляем в начало цепочки
    ht->table[index] = new_macro;
}

void rehash(hashtable *ht) {
    int old_size = ht->size;
    ht->size *= 2;  // Увеличиваем размер таблицы
    macro **new_table = (macro **) malloc(ht->size * sizeof(macro *));

    // Инициализация новой таблицы
    for (int i = 0; i < ht->size; i++) {
        new_table[i] = NULL;
    }

    // Перераспределяем все элементы в новую таблицу
    for (int i = 0; i < old_size; i++) {
        macro *current = ht->table[i];
        while (current != NULL) {
            insert(new_table, current->key, current->value);  // добавляем в новую таблицу
            current = current->next;
        }
    }

    // Освобождаем старую таблицу и обновляем указатель
    free(ht->table);
    ht->table = new_table;
}

state add_defines(macro ***out_data, FILE *input_file) {

    hashtable *ht = (hashtable *) malloc(sizeof(hashtable));
    if (ht == NULL) {
        return MEMORY_ERROR;
    }
    ht->size = HASHSIZE;

    ht->table = (macro **) malloc(sizeof(macro));
    if (ht->table == NULL) {
        FREE_AND_NULL(ht)
        return MEMORY_ERROR;
    }

    while (!feof(input_file)) {
        char *key, *value, *define;

        key = (char *) malloc(sizeof(char) * 128);
        value = (char *) malloc(sizeof(char) * 128);
        define = (char *) malloc(sizeof(char) * (7 + 1));

        if (key == NULL || value == NULL || define == NULL) {
            FREE_AND_NULL(key)
            FREE_AND_NULL(value)
            FREE_AND_NULL(define)
            for (int i = 0; i < ht->size; ++i) {
                FREE_AND_NULL(ht->table[i]->key)
                FREE_AND_NULL(ht->table[i]->value)
            }
            FREE_AND_NULL(ht)
            return MEMORY_ERROR;
        }

        fscanf(input_file, "%s%s%s", define, key, value);
        while (strcmp("#define", define) == 0) {
            insert(ht, key, value);

            key = (char *) malloc(sizeof(char) * 128);
            value = (char *) malloc(sizeof(char) * 128);
            define = (char *) malloc(sizeof(char) * (7 + 1));
            fscanf(input_file, "%s%s%s", define, key, value);
        }
    }

    *out_data = ht->table;
    return OK;
}

//TODO придумать рекурсивную функцию по очищению цепочек
//void free_all(hashtable *ht) {
//
//    FREE_AND_NULL(stud_array)
//}
