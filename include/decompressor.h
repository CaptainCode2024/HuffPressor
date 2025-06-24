#ifndef DECOMPRESSOR_H
#define DECOMPRESSOR_H

#include "bitReader.h"
#include "huffmanTree.h"
#include <string>
#include <fstream>

// Handles reading compressed data and reconstructing the original file
class Decompressor {
public:
    // High-level method: performs full decompression from file
    bool decompressFile(const std::string& inputFilename, const std::string& outputFilename);

private:
    // Reconstructs Huffman tree from serialized bit stream
    HuffmanNode* deserializeTree(BitReader& reader);

    // Traverses tree based on bitstream and writes original data
    void decode(BitReader& reader, std::ostream& output, HuffmanNode* root);
};

#endif // DECOMPRESSOR_H
