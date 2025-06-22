#include "compressor.h"
#include <iostream>

int main() {
    Compressor compressor;
    std::string filename = "../test.txt"; // You can change this to any file

    if (compressor.readFileAndBuildFrequency(filename)) {
        std::cout << "Byte Frequencies:\n";
        for (const auto& [byte, count] : compressor.getFrequencyMap()) {
            std::cout << "Byte: " << static_cast<int>(byte) << " -> Count: " << count << "\n";
        }
    } else {
        std::cerr << "Failed to process file.\n";
    }

    return 0;
}
