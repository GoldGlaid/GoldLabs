#include <iostream>

class logic_val_array {
private:
    unsigned int value;
public:
    logic_val_array(unsigned int num = 0) : value(num) {}

    unsigned int get_value() const {
        return value;
    }

    static logic_val_array inversion(const logic_val_array &first) {
        return logic_val_array(~(first.value));
    }

    static logic_val_array disjunction(const logic_val_array &first, const logic_val_array &second) {
        return logic_val_array(first.value | second.value);
    }

    static logic_val_array conjunction(const logic_val_array &first, const logic_val_array &second) {
        return logic_val_array(inversion(disjunction(inversion(first.value), inversion(second.value))));
    }

    static logic_val_array implication(const logic_val_array &first, const logic_val_array &second) {
        return logic_val_array(disjunction(inversion(first.value), second.value));
    }

    static logic_val_array coimplication(const logic_val_array &first, const logic_val_array &second) {
        return logic_val_array(inversion(disjunction(inversion(first.value), second.value)));
    }

    static logic_val_array XOR(const logic_val_array &first, const logic_val_array &second) {
        return logic_val_array(disjunction(
                conjunction(first.value, inversion(second.value)),
                conjunction(inversion(first.value), second.value)));
    }

    static logic_val_array equivalent(const logic_val_array &first, const logic_val_array &second) {
        return logic_val_array(inversion(XOR(first.value, second.value)));
    }

    static logic_val_array Pier_arrow(const logic_val_array &first, const logic_val_array &second) {
        return logic_val_array(inversion(disjunction(first.value, second.value)));
    }

    static logic_val_array Schaeffer(const logic_val_array &first, const logic_val_array &second) {
        return logic_val_array(inversion(conjunction(first.value, second.value)));
    }

    static bool equals(const logic_val_array &first, const logic_val_array &second) {
        return first.value == second.value;
    }

    static bool get_bit(const logic_val_array &first, unsigned int bit) {
        if (bit >= sizeof(unsigned int) * 8) {
            throw std::logic_error("Bit more than size of unsigned int number\n");
        }

        return (first.value >> bit) & 1;
    }

    static void convertation(char *string, const logic_val_array &first) {
        if (string == nullptr) {
            throw std::logic_error("Null ptr\n");
        }

        int i;
        const short number_with_bits = sizeof(unsigned int) * 8;

        for (i = 0; i < number_with_bits; ++i) {
            if ((first.value & (1 << i)) == 0)
                string[number_with_bits - i - 1] = '0';
            else
                string[number_with_bits - i - 1] = '1';
        }

        string[number_with_bits] = '\0';
    }

};

int main() {

    bool answer;
    logic_val_array a(10);
    logic_val_array b(12);
    char for_convert[33];

    logic_val_array c = logic_val_array::inversion(a);
    std::cout << "Inversion: " << "~" << a.get_value() << " = " << c.get_value() << "\n";
    c = logic_val_array::conjunction(a, b);
    std::cout << "Conjunction: " << a.get_value() << " & " << b.get_value() << " = " << c.get_value() << "\n";
    c = logic_val_array::disjunction(a, b);
    std::cout << "Disjunction: " << a.get_value() << " | " << b.get_value() << " = " << c.get_value() << "\n";
    c = logic_val_array::implication(a, b);
    std::cout << "Implication: " << a.get_value() << " -> " << b.get_value() << " = " << c.get_value() << "\n";
    c = logic_val_array::coimplication(a, b);
    std::cout << "Complication: " << a.get_value() << " <- " << b.get_value() << " = " << c.get_value() << "\n";
    c = logic_val_array::XOR(a, b);
    std::cout << "XOR: " << a.get_value() << " + " << b.get_value() << " = " << c.get_value() << "\n";
    c = logic_val_array::equivalent(a, b);
    std::cout << "Equivalent: " << a.get_value() << " ~ " << b.get_value() << " = " << c.get_value() << "\n";
    c = logic_val_array::Pier_arrow(a, b);
    std::cout << "Pier_arrow: " << a.get_value() << " | " << b.get_value() << " = " << c.get_value() << "\n";
    c = logic_val_array::Schaeffer(a, b);
    std::cout << "Schaeffer: " << a.get_value() << " || " << b.get_value() << " = " << c.get_value() << "\n";
    c = logic_val_array::equals(a, b);
    std::cout << "Check equals: " << a.get_value() << " == " << b.get_value() << " = " << c.get_value() << "\n";

    try {
        answer = logic_val_array::get_bit(b, 2);
        std::cout << "Check your bit: " << b.get_value() << " bit 2" << " = " << answer << "\n";

    } catch (std::logic_error &problem) {
        std::cout << "Error: " << problem.what();
    }

    try {
        logic_val_array::convertation(for_convert, a);
        std::cout << "Binary representation: " << a.get_value() << " = " << for_convert << "\n";
    } catch (std::logic_error const &problem) {
        std::cout << "Error: " << problem.what();
    }

    return 0;
}