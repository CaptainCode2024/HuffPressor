#include "compressor.h"
#include "huffmanTree.h"
#include <iostream>

int main() {
    Compressor compressor;

    // Input file to compress (relative path from build directory)
    std::string inputFilename = "../test.txt";
    std::string outputFilename = "../compressed.huff";

    // Step 1: Read input file and build frequency map
    if (!compressor.readFileAndBuildFrequency(inputFilename)) {
        std::cerr << "Failed to read input file.\n";
        return 1;
    }

    // Step 2: Build Huffman tree from the frequency map
    HuffmanTree tree;
    tree.build(compressor.getFrequencyMap());

    // Step 3: Generate Huffman codes for each byte
    const auto& codes = tree.getHuffmanCodes();

    // Step 4 & 5: Compress file and serialize tree + encoded data
    if (compressor.compressFile(inputFilename, outputFilename, codes, tree.getRoot())) {
        std::cout << "Compression successful.\n";
    } else {
        std::cerr << "Compression failed.\n";
    }

    return 0;
}
