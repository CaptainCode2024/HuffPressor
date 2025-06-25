#include "compressor.h"
#include "bitWriter.h"
#include <fstream>
#include <iostream>

/**
 * Reads the input file byte-by-byte and builds the frequency map.
 * This map will be used to build the Huffman tree.
 */
bool Compressor::readFileAndBuildFrequency(const std::string& filename) {
    std::ifstream input(filename, std::ios::binary);
    if (!input.is_open()) {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return false;
    }

    freqMap.clear();
    originalFileSize = 0;

    unsigned char byte;
    while (input.read(reinterpret_cast<char*>(&byte), 1)) {
        freqMap[byte]++;
        originalFileSize++;
    }

    input.close();
    return true;
}

/**
 * Returns the byte frequency map.
 * Used externally to build the Huffman tree.
 */
const std::unordered_map<unsigned char, int>& Compressor::getFrequencyMap() const {
    return freqMap;
}

/**
 * Returns the total size (in bytes) of the original file.
 * This is stored as metadata for reconstruction.
 */
uint64_t Compressor::getOriginalFileSize() const {
    return originalFileSize;
}

/**
 * Compresses the input file by replacing each byte with its corresponding Huffman code.
 * Writes output to compressed file along with metadata.
 */
bool Compressor::compressFile(const std::string& inputFilename,
                              const std::string& outputFilename,
                              const std::unordered_map<unsigned char, std::string>& codes,
                              HuffmanNode* root) {
    std::ifstream input(inputFilename, std::ios::binary);
    if (!input.is_open()) {
        std::cerr << "Error: Cannot open input file: " << inputFilename << "\n";
        return false;
    }

    std::ofstream output(outputFilename, std::ios::binary);
    if (!output.is_open()) {
        std::cerr << "Error: Cannot create output file: " << outputFilename << "\n";
        input.close();
        return false;
    }

    BitWriter writer(output);

    // 1 Write the Huffman Tree as a header
    writer.writeTree(root);

    // 2 Write the original file size (64-bit big-endian)
    std::cout << "Writing original file size: " << originalFileSize << " bytes\n";
    for (int i = 7; i >= 0; --i) {
        unsigned char sizeByte = static_cast<unsigned char>((originalFileSize >> (i * 8)) & 0xFF);
        writer.writeByte(sizeByte); // Ensure it's written through the buffer
        std::cout << "Size Byte[" << (7 - i) << "]: " << static_cast<int>(sizeByte) << "\n";
    }

    // 3 Encode input bytes using Huffman codes
    unsigned char byte;
    while (input.read(reinterpret_cast<char*>(&byte), 1)) {
        const std::string& code = codes.at(byte);
        writer.writeBits(code);  // Buffered bit writing

        // Optional debug output
        std::cout << "Encoding '" << byte << "' → " << code << " (" << code.length() << " bits)\n";
    }

    // 4 Final flush to write remaining bits
    writer.flush();

    input.close();
    output.close();

    std::cout << "Compression complete. Output: " << outputFilename << "\n";
    return true;
}
