#ifndef GOLDLABS_FUNCTIONS_ETC_H
#define GOLDLABS_FUNCTIONS_ETC_H

#include <iostream>
#include <cmath>
#include <bitset>

class Binary_int final {
    int value{};

    static int add(int a, int b);

    static int negative(int a);

    static int substraction(int a, int b);

    static int multiplication(int a, int b);

    static int shift_to_left(int a, int shift);

    static int shift_to_right(int a, int shift);

public:
    Binary_int() = default;

    Binary_int(int a) : value(a) {}

    Binary_int operator-();

    Binary_int &operator++();

    Binary_int operator++(int);

    Binary_int &operator--();

    Binary_int operator--(int);

    Binary_int &operator+=(Binary_int const &right);

    Binary_int operator+(Binary_int const &right);

    Binary_int &operator-=(Binary_int const &right);

    Binary_int operator-(Binary_int const &right);

    Binary_int &operator*=(Binary_int const &right);

    Binary_int operator*(Binary_int const &right);

    Binary_int &operator<<=(Binary_int const &right);

    Binary_int operator<<(Binary_int const &right);

    Binary_int &operator>>=(Binary_int const &right);

    Binary_int operator>>(Binary_int const &right);

    friend std::ostream &operator<<(std::ostream &out, Binary_int const &right);

    int get_value() const { return value; }

    std::pair<Binary_int, Binary_int> get_pair() const;

};


#endif //GOLDLABS_FUNCTIONS_ETC_H
