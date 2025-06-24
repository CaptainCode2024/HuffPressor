#include "compressor.h"
#include "huffmanTree.h"
#include <iostream>

int main() {
    Compressor compressor;

    // Path to the input file (relative to build directory)
    std::string filename = "../test.txt";

    // Step 1: Read file and build frequency map
    if (compressor.readFileAndBuildFrequency(filename)) {
        std::cout << "Byte Frequencies:\n";
        for (const auto& [byte, count] : compressor.getFrequencyMap()) {
            std::cout << "Byte: " << static_cast<int>(byte)
                      << " -> Count: " << count << "\n";
        }
    } else {
        std::cerr << "Failed to process file.\n";
        return 1;
    }

    // Step 2: Build Huffman Tree
    HuffmanTree tree;
    tree.build(compressor.getFrequencyMap());
    std::cout << "Huffman Tree built successfully.\n";

    // Step 3: Generate Huffman Codes
    tree.generateCodes();
    const auto& codeMap = tree.getCodes();
    std::cout << "Generated Huffman Codes:\n";
    for (const auto& [byte, code] : codeMap) {
        std::cout << "Byte: " << static_cast<int>(byte) << " -> Code: " << code << "\n";
    }

    return 0;
}
