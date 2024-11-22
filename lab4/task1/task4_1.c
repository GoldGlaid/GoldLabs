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
    int numIntoChain;
    struct macro *next;
} macro;

typedef struct hashtable {
    int size;
    macro **table;
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

state add_defines(hashtable **out_data, FILE *input_file);

state get_word(FILE *file, char **outData, char *symbol);

unsigned int hash(char *key, int tableSize);

state insert(hashtable *ht, char *key, char *value);

void rehash(hashtable *ht);

void freeElement(macro *element);

void freeHashTable(hashtable *ht);


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
    hashtable *ht;

    // запись из файла в массив
    state result = add_defines(&ht, input_file);
    if (result == MEMORY_ERROR) {
        fclose(input_file);
        fclose(output_file);
        freeHashTable(ht);
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


unsigned int hash(char *key, int tableSize) {
    unsigned int hash_value = 0;
    while (*key) {
        hash_value = (hash_value * 62 + (*key - '0')) % tableSize;
        key++;
    }
    return hash_value;
}


state insert(hashtable *ht, char *key, char *value) {
    unsigned int index;
    macro *new_macro;
//    macro *tableElement;

    index = hash(key, ht->size);
    new_macro = (macro *) malloc(sizeof(macro));
    if (new_macro == NULL) {
        return MEMORY_ERROR;
    }
    new_macro->key = key;
    new_macro->value = value;

//    tableElement
    if (*ht->table[index] == NULL) {
        new_macro->numIntoChain = 0;
    } else {
        new_macro->numIntoChain = ++(ht->table[index]->numIntoChain);
    }
    new_macro->next = ht->table[index];  // добавляем в начало цепочки

    ht->table[index] = new_macro;
}

//void rehash(hashtable *ht) {
//    int old_size = ht->size;
//    ht->size *= 2;  // Увеличиваем размер таблицы
//    macro **new_table = (macro **) malloc(ht->size * sizeof(macro *));
//
//    // Инициализация новой таблицы
//    for (int i = 0; i < ht->size; i++) {
//        new_table[i] = NULL;
//    }
//
//    // Перераспределяем все элементы в новую таблицу
//    for (int i = 0; i < old_size; i++) {
//        macro *current = ht->table[i];
//        while (current != NULL) {
//            insert(new_table, current->key, current->value);  // добавляем в новую таблицу
//            current = current->next;
//        }
//    }
//
//    // Освобождаем старую таблицу и обновляем указатель
//    free(ht->table);
//    ht->table = new_table;
//}

void freeElement(macro *element) {
    FREE_AND_NULL(element->key)
    FREE_AND_NULL(element->value)
    FREE_AND_NULL(element)
}

void freeHashTable(hashtable *ht) {
    macro *element;
    macro *ptr_next = NULL;

    for (int i = 0; i < ht->size; ++i) {
        element = ht->table[i];
        do {
            ptr_next = element->next;
            freeElement(element);
        } while (ptr_next != NULL);
    }

    FREE_AND_NULL(ht->table)
    FREE_AND_NULL(ht);
}

state add_defines(hashtable **out_data, FILE *input_file) {

    hashtable *ht = (hashtable *) malloc(sizeof(hashtable));
    if (ht == NULL) {
        return MEMORY_ERROR;
    }
    ht->size = HASHSIZE;

    ht->table = (macro **) malloc(sizeof(macro **));
    if (ht->table == NULL) {
        FREE_AND_NULL(ht)
        return MEMORY_ERROR;
    }

    while (!feof(input_file)) {
        char *key, *value, *word;
        state result = 0;
        char symbol = '0';

        word = (char *) malloc(sizeof(char) * 128);
        key = (char *) malloc(sizeof(char) * 128);
        value = (char *) malloc(sizeof(char) * 128);

        if (key == NULL || value == NULL || word == NULL) {
            FREE_AND_NULL(key)
            FREE_AND_NULL(value)
            FREE_AND_NULL(word)
            freeHashTable(ht);
            return MEMORY_ERROR;
        }
        while (symbol != EOF) {
            result = get_word(input_file, &word, &symbol);
            if (result == MEMORY_ERROR) {
                FREE_AND_NULL(key)
                FREE_AND_NULL(value)
                FREE_AND_NULL(word)
                freeHashTable(ht);
                return MEMORY_ERROR;
            }
            if (strcmp(word, "#define") != 0)
                break;

            while (symbol != '\n') {
                result = get_word(input_file, &word, &symbol);
                if (result == MEMORY_ERROR) {
                    FREE_AND_NULL(key)
                    FREE_AND_NULL(value)
                    FREE_AND_NULL(word)
                    freeHashTable(ht);
                    return MEMORY_ERROR;
                }
                key = word;

                result = get_word(input_file, &word, &symbol);
                if (result == MEMORY_ERROR) {
                    FREE_AND_NULL(key)
                    FREE_AND_NULL(value)
                    FREE_AND_NULL(word)
                    freeHashTable(ht);
                    return MEMORY_ERROR;
                }

                value = word;
                insert(ht, key, value);
            }
        }


        *out_data = ht;
        return OK;
    }
}


state get_word(FILE *file, char **outData, char *symbol) {
    int size = 0;
    int capacity = 2;

    char *word = (char *) malloc(sizeof(char) * capacity);

    if (word == NULL) {
        return MEMORY_ERROR;
    }

    *symbol = (char) getc(file);

    while (*symbol != '\n' && *symbol != EOF && *symbol != ' ') {
        // Проверка на переполнение
        if (size == capacity) {
            capacity *= 2;
            char *buffer_realloc = (char *) realloc(word, sizeof(char) * capacity);
            if (buffer_realloc == NULL) {
                FREE_AND_NULL(word)
                return MEMORY_ERROR;
            }
            word = buffer_realloc;
        }
        word[size] = *symbol;
        size++;

        *symbol = (char) getc(file);
    }
    print
    word[size] = '\0';
    *outData = word;

    return OK;
}


state get_define(FILE *file, char **outData, char *symbol) {

}