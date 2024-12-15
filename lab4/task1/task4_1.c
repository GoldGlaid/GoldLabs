#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <limits.h>

#define HASHSIZE 128

typedef struct macro {
    char *key;
    char *value;
    char *fullHash;
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
} state;


state simulate_files(char *path_file1, char *path_file2);

hashtable *create_hashtable(int size);

unsigned int hash(char const *key, int tableSize);

state insert(hashtable *ht, char **key, char **value);

macro *find_macro(hashtable *ht, char const *key);

state rehash(hashtable *ht);

bool should_rehash(hashtable *ht);

void freeHashTable(hashtable *ht);

void freeElement(macro *element);

state add_defines(hashtable **out_data, FILE *input_file);

state process_text(FILE *input_file, FILE **output_file, hashtable *ht);

state get_word(FILE *input, char **buffer, char *c);


int main(int argc, char *argv[]) {
    state result;
    char *input_path, *output_path;
    FILE *input_file;
    FILE *output_file;
    hashtable *ht;


    if (argc != 3) {
        printf("ERROR: INVALID_INPUT.\n");
        return INVALID_INPUT;
    }

    input_path = argv[1];
    output_path = argv[2];

    if (simulate_files(input_path, output_path) == INVALID_FILES) {
        printf("ERROR: INVALID_FILES. The input file is equal to the output file.\n");
        return INVALID_FILES;
    }

    // Открываем файлы
    input_file = fopen(input_path, "r");
    if (input_file == NULL) {
        printf("ERROR: NOT_EXIST_FILE.\n");
        return NOT_EXIST_FILE;
    }

    output_file = fopen(output_path, "w");
    if (output_file == NULL) {
        fclose(input_file);
        printf("ERROR: NOT_EXIST_FILE.\n");
        return NOT_EXIST_FILE;
    }

    // Создаем хеш-таблицу
    ht = create_hashtable(HASHSIZE);

    // Чтение директив #define
    result = add_defines(&ht, input_file);
    if (result != OK) {
        fclose(input_file);
        fclose(output_file);
        freeHashTable(ht);

        if (result == MEMORY_ERROR) {
            printf("ERROR: MEMORY_ERROR.\n");
            return MEMORY_ERROR;
        }

        printf("ERROR: NULL_PTR.\n");
        return NULL_PTR;
    }


    if (should_rehash(ht)) {
        result = rehash(ht);
        if (result != OK) {
            fclose(input_file);
            fclose(output_file);
            freeHashTable(ht);

            if (result == NULL_PTR) {
                printf("ERROR: NULL_PTR.\n");
                return NULL_PTR;
            } else {
                printf("ERROR: MEMORY_ERROR.\n");
                return MEMORY_ERROR;
            }
        }
    }

    //Перенос указателя на начало файла
    fclose(input_file);
    input_file = fopen(input_path, "r");
    if (input_file == NULL) {
        fclose(output_file);
        freeHashTable(ht);
        printf("ERROR: NOT_EXIST_FILE.\n");
        return NOT_EXIST_FILE;
    }


    result = process_text(input_file, &output_file, ht);

    if (result == MEMORY_ERROR) {
        printf("ERROR: MEMORY_ERROR.\n");
    }

    fclose(input_file);
    fclose(output_file);
    freeHashTable(ht);
    return result;
}


state simulate_files(char *path_file1, char *path_file2) {
    char real_path1[BUFSIZ];
    char real_path2[BUFSIZ];

    if (!realpath(path_file1, real_path1) || !realpath(path_file2, real_path2))
        return INVALID_FILES;

    if (strcmp(real_path1, real_path2) == 0) {
        return INVALID_FILES;
    }

    return OK;
}


state add_defines(hashtable **out_data, FILE *input_file) {
    int capacity = 2;
    char symbol = '0';
    char *define_word, *key, *value;
    state result = OK;

    if (out_data == NULL || *out_data == NULL || input_file == NULL) {
        return NULL_PTR;
    }

    hashtable *ht = *out_data;

    while (!feof(input_file)) {

        define_word = (char *) malloc(sizeof(char) * capacity);
        key = (char *) malloc(sizeof(char) * capacity);
        value = (char *) malloc(sizeof(char) * capacity);

        if (define_word == NULL || key == NULL || value == NULL) {
            free(define_word);
            free(key);
            free(value);
            return MEMORY_ERROR;
        }

        result = get_word(input_file, &define_word, &symbol);
        if (result != OK) {
            free(define_word);
            free(key);
            free(value);
            return result == NULL_PTR ? NULL_PTR : MEMORY_ERROR;
        }


        if (strcmp(define_word, "#define") == 0) {

            // Читаем ключ
            result = get_word(input_file, &key, &symbol);
            if (result != OK) {
                free(define_word);
                free(key);
                free(value);
                return result == NULL_PTR ? NULL_PTR : MEMORY_ERROR;
            }

            // Читаем значение
            result = get_word(input_file, &value, &symbol);
            if (result != OK) {
                free(define_word);
                free(key);
                free(value);
                return result == NULL_PTR ? NULL_PTR : MEMORY_ERROR;
            }

            // Вставляем макрос в хеш-таблицу
            result = insert(ht, &key, &value);
            if (result == MEMORY_ERROR) {
                free(key);
                free(value);
                free(define_word);
                return MEMORY_ERROR;
            }
        }
        free(define_word);
    }
    return OK;
}


state process_text(FILE *input_file, FILE **output_file, hashtable *ht) {
    char *word;
    char symbol;
    int capacity = 2;
    state status;
    macro *m;

    if (input_file == NULL || *output_file == NULL || ht == NULL) {
        return MEMORY_ERROR;
    }

    while (!feof(input_file)) {
        word = (char *) malloc(sizeof(char) * capacity);
        if (word == NULL) {
            return MEMORY_ERROR;
        }

        status = get_word(input_file, &word, &symbol);
        if (status == MEMORY_ERROR) {
            free(word);
            return MEMORY_ERROR;
        }

        // обработка начальных define
        if (strcmp("#define", word) == 0) {
            fprintf(*output_file, "%s", word);
            fputc(' ', *output_file);

            status = get_word(input_file, &word, &symbol);
            if (status == MEMORY_ERROR) {
                free(word);
                return MEMORY_ERROR;
            }
            fprintf(*output_file, "%s", word);
            fputc(' ', *output_file);
            free(word);
            continue;
        }

        m = find_macro(ht, word);
        if (m != NULL) {
            fprintf(*output_file, "%s", m->value);
        } else {
            fprintf(*output_file, "%s", word);
        }

        // сохраняем пробелы и другие символы
        if (symbol == ' ' || symbol == '\n' || symbol == '\t') {
            fputc(symbol, *output_file);
        }
        if (symbol == '\r') {
            fputc(symbol, *output_file);
            symbol = '\n';
            fputc(symbol, *output_file);
        }
        free(word);
    }

    return OK;
}


hashtable *create_hashtable(int size) {
    hashtable *ht = (hashtable *) malloc(sizeof(hashtable));
    if (ht == NULL) {
        return NULL;
    }

    ht->size = size;
    ht->table = (macro **) malloc(sizeof(macro *) * size);

    if (ht->table == NULL) {
        free(ht);
        return NULL;
    }

    for (int i = 0; i < size; i++) {
        ht->table[i] = NULL;
    }

    return ht;
}

void freeElement(macro *element) {
    free(element->key);
    free(element->value);
    free(element->fullHash);
    free(element);
}

void freeHashTable(hashtable *ht) {
    if (ht == NULL) return;

    macro *element;
    macro *ptr_next;

    for (int i = 0; i < ht->size; i++) {
        element = ht->table[i];
        while (element != NULL) {
            ptr_next = element->next;
            freeElement(element);
            element = ptr_next;
        }
    }

    free(ht->table);
    free(ht);
}

unsigned int hash(char const *key, int tableSize) {
    unsigned int hash_value = 0;
    while (*key) {
        char c = *key;
        if ('0' <= c && c <= '9') {
            hash_value = hash_value * 62 + (c - '0');
        } else if ('A' <= c && c <= 'Z') {
            hash_value = hash_value * 62 + (c - 'A' + 10);
        } else if ('a' <= c && c <= 'z') {
            hash_value = hash_value * 62 + (c - 'a' + 36);
        }
        key++;
    }
    return hash_value % tableSize;
}

state insert(hashtable *ht, char **key, char **value) {
    unsigned int index = hash(*key, ht->size);

    macro *new_macro = (macro *) malloc(sizeof(macro));
    if (new_macro == NULL) {
        return MEMORY_ERROR;
    }

    new_macro->key = *key;
    new_macro->value = *value;
    new_macro->next = ht->table[index];

    ht->table[index] = new_macro;

    return OK;
}


state rehash(hashtable *ht) {
    int old_size;
    macro **old_table;
    macro *element, *old_element;

    if (ht == NULL)
        return NULL_PTR;

    old_size = ht->size;
    old_table = ht->table;
    ht->size *= 2;

    ht->table = (macro **) calloc(ht->size, sizeof(macro *));
    if (ht->table == NULL)
        return MEMORY_ERROR;

    // Инициализация новой таблицы
    for (int i = 0; i < ht->size; i++) {
        ht->table[i] = NULL;
    }

    // Перенос элементов из старой таблицы в новую
    for (int i = 0; i < old_size; ++i) {
        old_element = old_table[i];
        while (old_element != NULL) {
            // Переносим указатели key и value в новую таблицу без их копирования
            insert(ht, &old_element->key, &old_element->value);

            // Освобождаем старый элемент
            element = old_element;
            old_element = old_element->next;
            // Освобождаем только сам элемент, строки останутся в новой таблице
            free(element);
        }
    }

    // Освобождаем старую таблицу
    free(old_table);

    return OK;
}



bool should_rehash(hashtable *ht) {
    macro *current;
    int chain_length;
    int min_chain_length = INT_MAX;
    int max_chain_length = 1;

    for (int i = 0; i < ht->size; i++) {
        chain_length = 0;
        current = ht->table[i];
        while (current != NULL) {
            chain_length++;
            current = current->next;
        }

        if (chain_length > max_chain_length) {
            max_chain_length = chain_length;
        }
        if (chain_length < min_chain_length) {
            min_chain_length = chain_length;
        }
    }

    return max_chain_length >= 2 * min_chain_length;
}


macro *find_macro(hashtable *ht, char const *key) {
    unsigned int index;
    macro *current;

    if (ht == NULL || key == NULL) {
        return NULL;
    }

    index = hash(key, ht->size);
    current = ht->table[index];

    while (current != NULL) {
        if (current->key != NULL && strcmp(current->key, key) == 0) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}


state get_word(FILE *input, char **buffer, char *c) {
    int capacity = 2;
    int index;
    char *temp_buffer;

    if (input == NULL || *buffer == NULL)
        return NULL_PTR;

    index = 0;
    *c = (char) fgetc(input);

    while (*c != ' ' && *c != '\n' && *c != '\t' && *c != EOF) {
        (*buffer)[index++] = *c;
        if (capacity == index) {
            capacity *= 2;
            temp_buffer = (char *) realloc(*buffer, capacity);
            if (temp_buffer == NULL) {
                free(*buffer);
                return MEMORY_ERROR;
            }
            *buffer = temp_buffer;
        }
        *c = (char) fgetc(input);
    }

    (*buffer)[index] = '\0';
    return OK;
}
