#include "compressor.h"
#include <fstream>
#include <iostream>

bool Compressor::readFileAndBuildFrequency(const std::string& filename) {
    std::ifstream input(filename, std::ios::binary); // Open file in raw binary mode
    if (!input.is_open()) {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return false;
    }

    unsigned char byte;
    // Read file byte by byte
    while (input.read(reinterpret_cast<char*>(&byte), 1)) {
        freqMap[byte]++; // Update frequency count
    }

    input.close();
    return true;
}

const std::unordered_map<unsigned char, int>& Compressor::getFrequencyMap() const {
    return freqMap;
}
