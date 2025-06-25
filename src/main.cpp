#include "compressor.h"
#include "decompressor.h"
#include "huffmanTree.h"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <string>

/**
 * Main driver for HuffPressor project.
 * Performs compression, decompression, and verifies data integrity.
 */
int main() {
    Compressor compressor;
    HuffmanTree tree;

    // File paths (under data/ directory)
    const std::string inputFilename        = "../data/test.txt";
    const std::string compressedFilename   = "../data/compressed.huff";
    const std::string decompressedFilename = "../data/decompressed.txt";

    // Step 1: Read and analyze input
    if (!compressor.readFileAndBuildFrequency(inputFilename)) {
        std::cerr << "Failed to read or analyze input file.\n";
        return 1;
    }

    // Step 2: Build Huffman tree
    tree.build(compressor.getFrequencyMap());

    // Step 3: Generate binary codes for each byte
    tree.generateCodes();

    // Step 4: Compress using generated codes
    if (!compressor.compressFile(inputFilename, compressedFilename, tree.getHuffmanCodes(), tree.getRoot())) {
        std::cerr << "Compression failed.\n";
        return 1;
    }

    std::cout << "Compression successful.\n";

    // Step 5: Decompress and rebuild original file
    Decompressor decompressor;
    if (!decompressor.decompressFile(compressedFilename, decompressedFilename)) {
        std::cerr << "Decompression failed.\n";
        return 1;
    }

    std::cout << "Decompression successful.\n";

    // Step 6: Verify file integrity by comparing decompressed and original input
    if (compareFiles(inputFilename, decompressedFilename)) {
        std::cout << "Files match! Compression and decompression verified.\n";
    } else {
        std::cerr << "Files do NOT match. Data integrity compromised.\n";
    }

    return 0;
}
