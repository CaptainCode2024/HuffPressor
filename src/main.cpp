#include "compressor.h"
#include "decompressor.h"
#include "huffmanTree.h"
#include "utils.h"
#include "config.h"

#include <iostream>
#include <fstream>
#include <string>


int main() {
    Compressor compressor;
    HuffmanTree tree;

    // File paths (under data/ directory)
    const std::string inputFilename        = "../data/test.txt";
    const std::string compressedFilename   = "../data/compressed.huff";
    const std::string decompressedFilename = "../data/decompressed.txt";

    // Step 1: Read and analyze input
    if (!compressor.readFileAndBuildFrequency(inputFilename)) {
#if ENABLE_LOGGING
        std::cerr << "Failed to read or analyze input file.\n";
#endif
        return 1;
    }

    // Step 2: Build Huffman tree
    tree.build(compressor.getFrequencyMap());

    // Step 3: Generate binary codes for each byte
    tree.generateCodes();

    // Step 4: Compress using generated codes
    if (!compressor.compressFile(inputFilename, compressedFilename, tree.getHuffmanCodes(), tree.getRoot())) {
#if ENABLE_LOGGING
        std::cerr << "Compression failed.\n";
#endif
        return 1;
    }

#if ENABLE_LOGGING
    std::cout << "Compression successful.\n";
#endif

    // Step 5: Decompress and rebuild original file
    Decompressor decompressor;
    if (!decompressor.decompressFile(compressedFilename, decompressedFilename)) {
#if ENABLE_LOGGING
        std::cerr << "Decompression failed.\n";
#endif
        return 1;
    }

#if ENABLE_LOGGING
    std::cout << "Decompression successful.\n";
#endif

    // Step 6: Verify file integrity
    if (compareFiles(inputFilename, decompressedFilename)) {
#if ENABLE_LOGGING
        std::cout << "Files match! Compression and decompression verified.\n";
#endif
    } else {
#if ENABLE_LOGGING
        std::cerr << "Files do NOT match. Data integrity compromised.\n";
#endif
    }

    return 0;
}
