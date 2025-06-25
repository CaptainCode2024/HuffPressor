#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <unordered_map>
#include <string>
#include <cstdint>

// Forward declaration
class HuffmanNode;

class Compressor {
public:
    bool readFileAndBuildFrequency(const std::string& filename);
    const std::unordered_map<unsigned char, int>& getFrequencyMap() const;
    uint64_t getOriginalFileSize() const;

    bool compressFile(const std::string& inputFilename,
                      const std::string& outputFilename,
                      const std::unordered_map<unsigned char, std::string>& codes,
                      HuffmanNode* root);

private:
    std::unordered_map<unsigned char, int> freqMap;
    uint64_t originalFileSize = 0;
};

#endif // COMPRESSOR_H