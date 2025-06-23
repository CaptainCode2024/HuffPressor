#include <iostream>
#include <string>

#include "compressor.h"
#include "huffmanTree.h"

int main() {
    Compressor compressor;

    // Path to the input file (relative to build directory)
    std::string filename = std::string("../test.txt");

    // Step 1: Read file and build frequency map
    if (compressor.readFileAndBuildFrequency(filename)) {
        std::cout << "Byte Frequencies:\n";
        for (const auto& [byte, count] : compressor.getFrequencyMap()) {
            std::cout << "Byte: " << static_cast<int>(byte)
                      << " -> Count: " << count << "\n";
        }
    } else {
        std::cerr << "Failed to process file.\n";
    }

    HuffmanTree tree;
    tree.build(compressor.getFrequencyMap());
    std::cout << "Huffman Tree built successfully.\n";

    return 0;
}
