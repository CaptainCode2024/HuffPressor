#include "decompressor.h"
#include <fstream>
#include <iostream>

// Public method to decompress a file using Huffman decoding
bool Decompressor::decompressFile(const std::string& inputFilename, const std::string& outputFilename) {
    std::ifstream input(inputFilename, std::ios::binary);
    if (!input.is_open()) {
        std::cerr << "Error opening compressed file: " << inputFilename << "\n";
        return false;
    }

    std::ofstream output(outputFilename, std::ios::binary);
    if (!output.is_open()) {
        std::cerr << "Error opening output file: " << outputFilename << "\n";
        return false;
    }

    BitReader reader(input);
    HuffmanNode* root = deserializeTree(reader); // Step 1: Rebuild Huffman tree
    if (!root) {
        std::cerr << "Failed to deserialize Huffman Tree.\n";
        return false;
    }

    decode(reader, output, root); // Step 2: Decode bitstream
    return true;
}

// Reconstruct Huffman Tree from encoded pre-order structure
HuffmanNode* Decompressor::deserializeTree(BitReader& reader) {
    bool bit;
    if (!reader.readBit(bit)) return nullptr;

    // Leaf node: read and return byte value
    if (bit) {
        unsigned char byte;
        if (!reader.readByte(byte)) return nullptr;
        return new HuffmanNode(byte, 0); // Frequency not needed for decoding
    }

    // Internal node: recursively build left and right subtrees
    HuffmanNode* left = deserializeTree(reader);
    HuffmanNode* right = deserializeTree(reader);
    return new HuffmanNode(0, left, right);
}

// Traverse Huffman tree to decode encoded data from bit stream
void Decompressor::decode(BitReader& reader, std::ostream& output, HuffmanNode* root) {
    HuffmanNode* current = root;
    bool bit;

    while (reader.readBit(bit)) {
        current = bit ? current->right : current->left;

        // Reached a leaf node, output corresponding byte
        if (current->isLeaf()) {
            output.put(current->byte);
            current = root; // Reset for next symbol
        }
    }
}
