#include "compressor.h"
#include <fstream>
#include <iostream>

// Reads the input file and builds a frequency map of each byte
bool Compressor::readFileAndBuildFrequency(const std::string& filename) {
    std::ifstream input(filename, std::ios::binary); // Open file in binary mode
    if (!input.is_open()) {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return false;
    }

    unsigned char byte;
    // Read the file byte by byte
    while (input.read(reinterpret_cast<char*>(&byte), 1)) {
        freqMap[byte]++; // Count frequency of each byte
    }

    input.close();
    return true;
}

// Getter for the frequency map
const std::unordered_map<unsigned char, int>& Compressor::getFrequencyMap() const {
    return freqMap;
}
