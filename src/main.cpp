#include "compressor.h"
#include "decompressor.h"
#include "huffmanTree.h"
#include "utils.h"
#include "config.h"

#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[]) {
    // Validate argument count; Expecting exactly 3 arguments after program name
    if (argc != 4) {
        std::cerr << "Usage:\n"
                  << "  " << argv[0] << " -c <input_file> <compressed_file>\n"
                  << "  " << argv[0] << " -d <compressed_file> <output_file>\n";
        return 1;
    }

    std::string mode = argv[1];       // Operation mode: -c (compress) or -d (decompress)
    std::string inputFile = argv[2];  // Input file path
    std::string outputFile = argv[3]; // Output file path

    if (mode == "-c") {
        // === COMPRESSION PATH ===
        Compressor compressor;
        HuffmanTree tree;

        // Step 1: Parse input and generate byte frequency map
        if (!compressor.readFileAndBuildFrequency(inputFile)) {
#if ENABLE_LOGGING
            std::cerr << "Failed to read or analyze input file.\n";
#endif
            return 1;
        }

        // Step 2: Construct Huffman Tree from frequency map
        tree.build(compressor.getFrequencyMap());

        // Step 3: Generate binary Huffman codes for each unique byte
        tree.generateCodes();

        // Step 4: Compress input using generated codes and Huffman tree
        if (!compressor.compressFile(inputFile, outputFile, tree.getHuffmanCodes(), tree.getRoot())) {
#if ENABLE_LOGGING
            std::cerr << "Compression failed.\n";
#endif
            return 1;
        }

#if ENABLE_LOGGING
        std::cout << "Compression successful.\n";
#endif

    } else if (mode == "-d") {
        // === DECOMPRESSION PATH ===
        Decompressor decompressor;

        // Decompress the file and reconstruct original content
        if (!decompressor.decompressFile(inputFile, outputFile)) {
#if ENABLE_LOGGING
            std::cerr << "Decompression failed.\n";
#endif
            return 1;
        }

#if ENABLE_LOGGING
        std::cout << "Decompression successful.\n";
#endif

    } else {
        // Invalid operation flag passed
        std::cerr << "Invalid mode: " << mode << "\n";
        std::cerr << "Use -c to compress or -d to decompress.\n";
        return 1;
    }

    return 0;
}
