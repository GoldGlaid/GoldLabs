#include "functions_etc.h"

int Binary_int::add(int a, int b) {
    while (b != 0) {
        int temp = a ^ b;
        b = (a & b) << 1;
        a = temp;
    }

    return a;
}

int Binary_int::negative(int a) {
    return add(~a, 1);
}

int Binary_int::substraction(int a, int b) {
    return add(a, negative(b));
}

int Binary_int::multiplication(int a, int b) {
    int res = 0;
    if (b < 0) {
        a = negative(a);
        b = negative(b);
    }

    while (b != 0) {
        if (b & 1) {
            res = add(res, a);
        }
        a <<= 1;
        b >>= 1;
    }

    return res;
}

int Binary_int::shift_to_left(int a, int b) {
    return a << b;
}

int Binary_int::shift_to_right(int a, int b) {
    return a >> b;
}

Binary_int Binary_int::operator-() {
    int result = negative(value);
    return Binary_int(result);
}

Binary_int &Binary_int::operator++() {
    value = add(value, 1);
    return *this;
}

Binary_int Binary_int::operator++(int) {
    Binary_int tmp(*this);
    value = add(value, 1);
    return tmp;
}

Binary_int &Binary_int::operator--() {
    value = substraction(value, 1);
    return *this;
}

Binary_int Binary_int::operator--(int) {
    Binary_int tmp(value);
    value = substraction(value, 1);
    return tmp;
}

Binary_int &Binary_int::operator+=(Binary_int const &right) {
    value = add(value, right.value);
    return *this;
}

Binary_int Binary_int::operator+(Binary_int const &right) {
    int result = add(value, right.value);
    return Binary_int(result);
}

Binary_int &Binary_int::operator-=(Binary_int const &right) {
    value = substraction(value, right.value);
    return *this;
}

Binary_int Binary_int::operator-(Binary_int const &right) {
    int result = substraction(value, right.value);
    return Binary_int(result);
}

Binary_int &Binary_int::operator*=(Binary_int const &right) {
    value = multiplication(value, right.value);
    return *this;
}

Binary_int Binary_int::operator*(Binary_int const &right) {
    int result = multiplication(value, right.value);
    return Binary_int(result);
}

Binary_int &Binary_int::operator<<=(Binary_int const &right) {
    value = shift_to_left(value, right.value);
    return *this;
}

Binary_int Binary_int::operator<<(Binary_int const &right) {
    int result = shift_to_left(value, right.value);
    return Binary_int(result);
}

Binary_int &Binary_int::operator>>=(Binary_int const &right) {
    value = shift_to_right(value, right.value);
    return *this;
}

Binary_int Binary_int::operator>>(Binary_int const &right) {
    int result = shift_to_right(value, right.value);
    return Binary_int(result);
}

std::ostream &operator<<(std::ostream &out, Binary_int const &right) {
    out << std::bitset<sizeof(right.value) * 8>(right.value);
    return out;
}

std::pair<Binary_int, Binary_int> Binary_int::get_pair() const {
    short shift = sizeof(value) << 2;
    std::pair<Binary_int, Binary_int> p;
    p.first = Binary_int(value & ((negative(1) >> shift) << shift));
    p.second = Binary_int(value & (add((1 << shift), negative(1))));
    return p;
}