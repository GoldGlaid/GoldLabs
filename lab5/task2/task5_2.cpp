#include <iostream>
#include <vector>
#include <bitset>
#include <fstream>
#include <filesystem>
#include <cstddef>

class encoder final{
private:
    int x, y;
    std::vector<std::byte> key, S;

    int to_int(std::byte b) const {
        return std::to_integer<int>(b);
    }

    void init() {
        S.resize(256);
        int key_length = key.size();

        for (int i = 0; i < 256; ++i) {
            S[i] = std::byte(i);
        }

        int j = 0;
        for (int i = 0; i < 256; ++i) {
            j = (j + to_int(S[i]) + to_int(key[i % key_length])) % 256;
            std::swap(S[i], S[j]);
        }
        x = 0;
        y = 0;
    }

    std::byte get_s_item() {
        x = (x + 1) % 256;
        y = (y + to_int(S[x])) % 256;
        std::swap(S[x], S[y]);
        return S[(to_int(S[x]) + to_int(S[y])) % 256];
    }

public:
    encoder(const std::vector<std::byte>& new_key) : key(new_key), x(0), y(0) {
        if (key.empty()) {
            throw std::invalid_argument("Key can not be empty\n");
        }
    }

    void set_key(const std::vector<std::byte>& new_key) {
        if (new_key.empty()) {
            throw std::invalid_argument("Key can not be empty\n");
        }

        key = new_key;
        init();
    }

    void encode(const std::string& input_file, const std::string& output_file) {

        std::filesystem::path input_path{input_file};
        if (!std::filesystem::exists(input_path)) {
            throw std::runtime_error("Input path is not exists\n");
        }

        std::filesystem::path output_path{output_file};
        if (!std::filesystem::exists(output_path)) {
            throw std::runtime_error("Output path is not exists\n");
        }

        if (std::filesystem::equivalent(input_path, output_path)) {
            throw std::runtime_error("Input and output files can not be the same\n");
        }

        std::ifstream input_stream(input_file);
        if (!input_stream.is_open()) {
            throw std::runtime_error("Failed to open input file\n");
        }

        std::ofstream output_stream(output_file);
        if (!output_stream.is_open()) {
            input_stream.close();
            throw std::runtime_error("Failed to open output file\n");
        }

        init();

        char byte;
        while (input_stream.get(byte)) {
            std::byte input_byte = std::byte(static_cast<unsigned char>(byte));
            std::byte encoded_byte = input_byte ^ get_s_item();
            output_stream.put(static_cast<unsigned char>(to_int(encoded_byte)));
        }

        input_stream.close();
        output_stream.close();
    }

    static std::vector<std::byte> generate_key(size_t size) {
        srand(time(NULL));

        std::vector<std::byte> key(size);
        for (int i = 0; i < size; ++i) {
            key[i] = std::byte(rand() % 256);
        }
        return key;
    }
};

int main() {
    try {
        std::vector<std::byte> key = encoder::generate_key(32);

        encoder encoder(key);

        std::string input_file = {R"(img.png)"};
        std::string encryption_file = {R"(encode.txt)"};
        std::string decryption_file = {R"(decode.png)"};


        encoder.encode(input_file, encryption_file);
        std::cout << "File encryption successfully\n";

        encoder.encode(encryption_file, decryption_file);
        std::cout << "File decryption successfully\n";

    } catch (const std::exception& error) {
        std::cerr << "ERROR: " << error.what() << std::endl;
    }

    return 0;
}