#include <stdio.h>

typedef enum STATE_ERRORS ( OK, INVALID INPUT, INVALID FLAG, INVALID NUMBER);

//TODO сделать функцию на проверку строки как число,проверку числа на отрицательность

int is_digit(const char * str_num, int base) {
}

int is_easy_digit(const int x) {
    if (!(x > 0)) {
        return 0;
    }
    // TODO сделать условие i < x в корне + 1
    for (int i = 2; i < x; ++i) {
        if (x % i == 0) {
        return 0;
        }
    }
    return 1;
}

int main(int argc, char *argv[]) {

    // проверка на кол-во аргументов
    if (argc != 3) {
        printf("ERROR: INVALID INPUT/n");
        return INVALID INPUT;
    }
    // проверка на верность введённого флага
    if (!(argv[1][0] == '-' || argv[1][0] == '/') && argv[1][2] == '\0') {
        printf("ERROR: INVALID FLAG/n");
        return INVALID FLAG;
    }
    // проверка на верность введённого числа
    if (!is_digit(argv[2], 10)) {
        printf("ERROR: INVALID NUMBER/n");
        return INVALID NUMBER;
    }
    
    // Флаг /h
    if (argv[1][1] == 'h') {
        // x - число
        enum flag(FALSE, TRUE);
        flag f = FALSE

        if (x > 100){
            printf("ERROR: INVALID NUMBER, number can't be > 100/n");
        return INVALID NUMBER;
        }
        // TODO возможно изменить условие с     флагом
        for (int i = 1; i <= 100; i++) {
            if (i % x == 0) {
                printf("%d/n", i);
                f = TRUE;
            }
        }
        if (!f) {
            printf("No multiples of x were found/n");
            return OK;
        }
    }

    // флаг p
    if (argv[1][1] == 'p') {
        // x - число 
        if (is_easy_digit(x)) {
            printf("The number is simple/n");
        }
        else {
            printf("The number is not simple/n);
        }
        return OK;
    }
}


