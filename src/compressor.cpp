#include "compressor.h"
#include "bitWriter.h"
#include "config.h"

#include <fstream>
#include <iostream>


bool Compressor::readFileAndBuildFrequency(const std::string& filename) {
    std::ifstream input(filename, std::ios::binary);
    if (!input.is_open()) {
#if ENABLE_LOGGING
        std::cerr << "Error: Could not open file " << filename << "\n";
#endif
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

const std::unordered_map<unsigned char, int>& Compressor::getFrequencyMap() const {
    return freqMap;
}

uint64_t Compressor::getOriginalFileSize() const {
    return originalFileSize;
}

bool Compressor::compressFile(const std::string& inputFilename,
                              const std::string& outputFilename,
                              const std::unordered_map<unsigned char, std::string>& codes,
                              HuffmanNode* root) {
    std::ifstream input(inputFilename, std::ios::binary);
    if (!input.is_open()) {
#if ENABLE_LOGGING
        std::cerr << "Error: Cannot open input file: " << inputFilename << "\n";
#endif
        return false;
    }

    std::ofstream output(outputFilename, std::ios::binary);
    if (!output.is_open()) {
#if ENABLE_LOGGING
        std::cerr << "Error: Cannot create output file: " << outputFilename << "\n";
#endif
        input.close();
        return false;
    }

    BitWriter writer(output);

    // Write Huffman Tree
    writer.writeTree(root);

    // Write original file size (64-bit big-endian)
#if ENABLE_LOGGING
    std::cout << "Writing original file size: " << originalFileSize << " bytes\n";
#endif
    for (int i = 7; i >= 0; --i) {
        unsigned char sizeByte = static_cast<unsigned char>((originalFileSize >> (i * 8)) & 0xFF);
        writer.writeByte(sizeByte);
#if ENABLE_LOGGING
        std::cout << "Size Byte[" << (7 - i) << "]: " << static_cast<int>(sizeByte) << "\n";
#endif
    }

    // Encode input file using Huffman codes
    unsigned char byte;
    while (input.read(reinterpret_cast<char*>(&byte), 1)) {
        const std::string& code = codes.at(byte);
        writer.writeBits(code);
#if ENABLE_LOGGING
        std::cout << "Encoding '" << byte << "' → " << code << " (" << code.length() << " bits)\n";
#endif
    }

    writer.flush();

    input.close();
    output.close();

#if ENABLE_LOGGING
    std::cout << "Compression complete. Output: " << outputFilename << "\n";
#endif
    return true;
}
