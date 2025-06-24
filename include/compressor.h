#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <unordered_map>
#include <string>

class Compressor {
public:
    // Step 1: Reads a file in binary mode and counts byte frequencies
    bool readFileAndBuildFrequency(const std::string& filename);

    // Step 4: Compresses a file using provided Huffman codes and writes binary output
    bool compressFile(const std::string& inputFilename,
                      const std::string& outputFilename,
                      const std::unordered_map<unsigned char, std::string>& huffmanCodes);

    // Provides access to the frequency map for tree construction
    const std::unordered_map<unsigned char, int>& getFrequencyMap() const;

private:
    std::unordered_map<unsigned char, int> freqMap;
};

#endif // COMPRESSOR_H
