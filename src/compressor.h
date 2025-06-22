#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <unordered_map>
#include <string>

class Compressor {
    public:
        // Reads a file in binary mode and counts byte frequencies
        bool readFileAndBuildFrequency(const std::string& filename);

        // Expose the frequency map for later stages (tree building, etc.)
        const std::unordered_map<unsigned char, int>& getFrequencyMap() const;

    private:
        std::unordered_map<unsigned char, int> freqMap;
};

#endif // COMPRESSOR_H
