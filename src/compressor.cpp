#include "compressor.h"
#include <fstream>
#include <iostream>
#include <bitset>

// Step 1: Read input file and build byte frequency map
bool Compressor::readFileAndBuildFrequency(const std::string& filename) {
    std::ifstream input(filename, std::ios::binary); // Open in binary mode
    if (!input.is_open()) {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return false;
    }

    unsigned char byte;
    while (input.read(reinterpret_cast<char*>(&byte), 1)) {
        freqMap[byte]++; // Increment frequency count for each byte
    }

    input.close();
    return true;
}

// Step 4: Compress file using Huffman codes and write to binary output file
bool Compressor::compressFile(const std::string& inputFilename,
                               const std::string& outputFilename,
                               const std::unordered_map<unsigned char, std::string>& huffmanCodes) {
    std::ifstream input(inputFilename, std::ios::binary);
    if (!input.is_open()) {
        std::cerr << "Error: Could not open input file " << inputFilename << "\n";
        return false;
    }

    std::ofstream output(outputFilename, std::ios::binary);
    if (!output.is_open()) {
        std::cerr << "Error: Could not open output file " << outputFilename << "\n";
        return false;
    }

    std::string bitBuffer;
    unsigned char byte;

    // Read input file byte by byte and build the bitBuffer using Huffman codes
    while (input.read(reinterpret_cast<char*>(&byte), 1)) {
        bitBuffer += huffmanCodes.at(byte); // Append corresponding Huffman code
        while (bitBuffer.size() >= 8) {
            std::bitset<8> byteBits(bitBuffer.substr(0, 8)); // Take 8 bits
            bitBuffer.erase(0, 8);                           // Remove processed bits
            output.put(static_cast<unsigned char>(byteBits.to_ulong())); // Write to output
        }
    }

    // Handle remaining bits (pad with 0s if not multiple of 8)
    if (!bitBuffer.empty()) {
        bitBuffer.append(8 - bitBuffer.size(), '0'); // Padding
        std::bitset<8> lastByte(bitBuffer);
        output.put(static_cast<unsigned char>(lastByte.to_ulong()));
    }

    input.close();
    output.close();
    return true;
}

// Provide access to frequency map
const std::unordered_map<unsigned char, int>& Compressor::getFrequencyMap() const {
    return freqMap;
}
