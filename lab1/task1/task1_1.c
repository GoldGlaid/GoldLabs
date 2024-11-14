#include <stdio.h>
#include <string.h>

///Структуры в СИ
struct MyStr { // Тип данных
    int x, y;
    char ch;
    double d;
    char s[256];
    int * pInt;
}; // важно завершать ;

/// Для выделения памяти под структуру необходимо использовать sizeof !!!


///Объединение в C
union MyUn { //Объявление объединения
    int x;
    char s[4];
};


/// Самоccылочные структуры данных. Абстрактные типы данных (Динамические структуры данных)
/// ! В структуре разрешено иметь поле типа указатель на эту же структуру.
struct  Node {
    int data;
    struct Node * pNode;
};

int main(void) {
    ///объявление переменной
    struct MyStr st;

    // указатель на структуру
    struct MyStr *pSt;

//    (*pSt).y = 100;
    st.y = 100;
    pSt->y = 500;

    //Объединение
    union MyUn u;
    int i = 0;

    st.x = 10;
    st.y = st.x + 2;
    strcpy(st.s, "Hello");

    u.x = 52;
    while (i < 4)
        printf("%x", u.s[i++]);

    return 0;
}
