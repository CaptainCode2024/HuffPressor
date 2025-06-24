#include "compressor.h"
#include "bitWriter.h"
#include <fstream>
#include <iostream>

bool Compressor::readFileAndBuildFrequency(const std::string& filename) {
    std::ifstream input(filename, std::ios::binary);
    if (!input.is_open()) {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return false;
    }

    unsigned char byte;
    while (input.read(reinterpret_cast<char*>(&byte), 1)) {
        freqMap[byte]++; // Count frequency of each byte
    }

    input.close();
    return true;
}

const std::unordered_map<unsigned char, int>& Compressor::getFrequencyMap() const {
    return freqMap;
}

bool Compressor::compressFile(const std::string& inputFilename,
                               const std::string& outputFilename,
                               const std::unordered_map<unsigned char, std::string>& codes,
                               HuffmanNode* root) {
    std::ifstream input(inputFilename, std::ios::binary);
    if (!input.is_open()) {
        std::cerr << "Error: Cannot open input file\n";
        return false;
    }

    std::ofstream output(outputFilename, std::ios::binary);
    if (!output.is_open()) {
        std::cerr << "Error: Cannot create output file\n";
        return false;
    }

    BitWriter writer(output);
    writer.writeTree(root); // Serialize Huffman tree

    unsigned char byte;
    while (input.read(reinterpret_cast<char*>(&byte), 1)) {
        writer.writeBits(codes.at(byte)); // Write encoded bits
    }

    writer.flush(); // Flush remaining bits
    input.close();
    output.close();
    return true;
}
