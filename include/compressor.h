#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <unordered_map>
#include <string>

class HuffmanNode; // Forward declaration to avoid circular dependency

class Compressor {
public:
    // Reads the input file and counts frequency of each byte
    bool readFileAndBuildFrequency(const std::string& filename);

    // Returns reference to the frequency map built from file
    const std::unordered_map<unsigned char, int>& getFrequencyMap() const;

    // Compresses the file using the generated Huffman codes
    bool compressFile(const std::string& inputFilename,
                      const std::string& outputFilename,
                      const std::unordered_map<unsigned char, std::string>& codes,
                      HuffmanNode* root);

private:
    std::unordered_map<unsigned char, int> freqMap; // Byte frequency map
};

#endif // COMPRESSOR_H
