#include <stdio.h>

typedef enum STATE_ERRORS ( OK, INVALID INPUT, INVALID FLAG, INVALID NUMBER);

//TODO сделать функцию на проверку строки как число
int is_digit(const char * str_num, int base) {
}

int main(int argc, char *argv[]) {

    // проверка на кол-во аргументов
    if (argc != 3) {
        printf("ERROR: INVALID INPUT");
        return INVALID INPUT;
    }
    // проверка на верность введённого флага
    if (!(argv[1][0] == '-' || argv[1][0] == '/') && argv[1][2] == '\0') {
        printf("ERROR: INVALID FLAG");
        return INVALID FLAG;
    }
    // проверка на верность введённого числа
    if (!is_digit(argv[2], 10)) {
        printf("ERROR: INVALID NUMBER");
        return INVALID NUMBER;
    }
}


