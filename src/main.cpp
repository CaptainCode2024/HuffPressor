#include "compressor.h"
#include "huffmanTree.h"
#include <iostream>

int main() {
    Compressor compressor;

    // Input file to be compressed (relative to build directory)
    std::string filename = "../test.txt";

    // Step 1: Build frequency map from input file
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

    // Step 2: Build Huffman Tree from frequency map
    HuffmanTree tree;
    tree.build(compressor.getFrequencyMap());
    std::cout << "Huffman Tree built successfully.\n";

    // Step 3: Generate Huffman codes
    const auto& codes = tree.getHuffmanCodes();
    std::cout << "Generated Huffman Codes:\n";
    for (const auto& [byte, code] : codes) {
        std::cout << "Byte: " << static_cast<int>(byte) << " -> Code: " << code << "\n";
    }

    // Step 4: Compress the input file using Huffman codes
    std::string outputFilename = "../compressed.huff";
    if (compressor.compressFile(filename, outputFilename, codes)) {
        std::cout << "File compressed successfully! Saved as: " << outputFilename << "\n";
    } else {
        std::cerr << "Compression failed.\n";
        return 1;
    }

    return 0;
}
