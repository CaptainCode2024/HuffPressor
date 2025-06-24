#include "compressor.h"
#include "decompressor.h"
#include "huffmanTree.h"

#include <iostream>
#include <string>

int main() {
    Compressor compressor;
    HuffmanTree tree;

    std::string inputFilename = "../test.txt";
    std::string compressedFilename = "compressed.huff";
    std::string decompressedFilename = "decompressed.txt";

    // Step 1: Read file and build frequency map
    if (!compressor.readFileAndBuildFrequency(inputFilename)) {
        std::cerr << "Failed to read and analyze input file.\n";
        return 1;
    }

    // Step 2: Build the Huffman Tree using the frequency map
    tree.build(compressor.getFrequencyMap());

    // Step 3: Generate binary codes for each byte
    tree.generateCodes();

    // Step 4 & 5: Compress the input file using codes and write encoded tree
    if (!compressor.compressFile(inputFilename, compressedFilename, tree.getHuffmanCodes(), tree.getRoot())) {
        std::cerr << "Compression failed.\n";
        return 1;
    }

    std::cout << "Compression successful.\n";

    // Step 6: Decompress the file and restore original content
    Decompressor decompressor;
    if (!decompressor.decompressFile(compressedFilename, decompressedFilename)) {
        std::cerr << "Decompression failed.\n";
        return 1;
    }

    std::cout << "Decompression successful.\n";

    return 0;
}
