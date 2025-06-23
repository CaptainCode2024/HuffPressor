#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <unordered_map>
#include <string>

// Handles reading a file and calculating byte frequencies
class Compressor {
public:
    // Reads the file in binary mode and counts how often each byte appears
    bool readFileAndBuildFrequency(const std::string& filename);

    // Returns a reference to the internal frequency map
    const std::unordered_map<unsigned char, int>& getFrequencyMap() const;

private:
    std::unordered_map<unsigned char, int> freqMap;
};

#endif // COMPRESSOR_H
