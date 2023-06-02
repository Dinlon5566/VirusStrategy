/*
 * @file Secure Delete
 * @author Dinlon5566
 * @date 2023-06-02
 *
 * This program implements a secure file deletion process, ensuring that sensitive data cannot be easily recovered.
 * It receives a file path as an argument, and overwrites the file with random data multiple times before deleting it.
 * By default, it overwrites the file 3 times, but this can be modified by changing the `overwriteTimes` argument in
 * the `SecureDelete` function.
 *
 * The main function handles command-line arguments and calls the `SecureDelete` function. If the number of command-line
 * arguments is not 2 (program name and target file), it provides usage information and exits. If an exception occurs
 * during file deletion (such as the file not being found), it catches the exception and outputs an error message.
 */

#include <fstream>
#include <string>
#include <random>
#include <vector>
#include <iostream>

void SecureDelete(std::string filepath, int overwriteTimes = 3) {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(0, 255);

    std::fstream file(filepath, std::ios::in | std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekp(0, std::ios::beg);

    std::vector<char> buffer(size);
    for (int j = 0; j < overwriteTimes; ++j) {
        for (std::streamsize i = 0; i < size; ++i) {
            buffer[i] = distribution(generator);
        }
        file.seekp(0, std::ios::beg);
        file.write(buffer.data(), size);
        file.flush();
    }

    file.close();
    remove(filepath.c_str());
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " targetfile\n";
        return 1;
    }

    try {
        SecureDelete(argv[1]);
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
