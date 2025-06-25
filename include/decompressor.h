#ifndef DECOMPRESSOR_H
#define DECOMPRESSOR_H

#include "bitReader.h"
#include "huffmanTree.h"
#include <string>
#include <fstream>
#include <cstdint>

class Decompressor {
public:
    bool decompressFile(const std::string& inputFilename, const std::string& outputFilename);

    uint64_t getOriginalFileSize() const;

    ~Decompressor();  // Destructor to free tree memory

private:
    HuffmanNode* deserializeTree(BitReader& reader);
    void decode(BitReader& reader, std::ostream& output, HuffmanNode* root, uint64_t originalSize);
    void freeTree(HuffmanNode* node);

    HuffmanNode* root = nullptr;  // Store root for cleanup
    uint64_t originalFileSize = 0;
};

#endif // DECOMPRESSOR_H
