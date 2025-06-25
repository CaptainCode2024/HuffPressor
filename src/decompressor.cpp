#include "decompressor.h"
#include "huffmanTree.h"
#include <fstream>
#include <iostream>
#include <cstdint>

/**
 * Destructor: Deallocates Huffman Tree nodes recursively.
 */
Decompressor::~Decompressor() {
    freeTree(root);
}

// Recursively deletes Huffman tree nodes
void Decompressor::freeTree(HuffmanNode* node) {
    if (!node) return;
    freeTree(node->left);
    freeTree(node->right);
    delete node;
}

/**
 * Main decompression function.
 * Reconstructs the Huffman tree, reads metadata, and decodes the compressed stream.
 */
bool Decompressor::decompressFile(const std::string& inputFilename, const std::string& outputFilename) {
    std::ifstream input(inputFilename, std::ios::binary);
    if (!input.is_open()) {
        std::cerr << "Failed to open compressed input file: " << inputFilename << "\n";
        return false;
    }

    std::ofstream output(outputFilename, std::ios::binary);
    if (!output.is_open()) {
        std::cerr << "Failed to open output file: " << outputFilename << "\n";
        return false;
    }

    BitReader reader(input);

    // Step 1: Deserialize Huffman Tree
    root = deserializeTree(reader);
    if (!root) {
        std::cerr << "Tree deserialization failed. Possibly corrupted input.\n";
        return false;
    }
    std::cout << "Huffman Tree deserialized successfully.\n";

    // Step 2: Align to byte boundary before reading file size
    reader.alignToByte();

    // Step 3: Read original file size (8 bytes, big-endian)
    uint64_t originalSize = 0;
    for (int i = 7; i >= 0; --i) {
        unsigned char sizeByte;
        if (!reader.readByte(sizeByte)) {
            std::cerr << "Failed to read file size metadata.\n";
            return false;
        }
        originalSize |= static_cast<uint64_t>(sizeByte) << (8 * i);
    }

    originalFileSize = originalSize;
    std::cout << "Original file size to decode: " << originalFileSize << " bytes\n";

    // Step 4: Decode bitstream using Huffman tree
    decode(reader, output, root, originalFileSize);

    std::cout << "Decompression complete. Output saved at: " << outputFilename << "\n";
    return true;
}

/**
 * Recursively rebuilds the Huffman Tree from bitstream.
 * Format: Internal Node = 0, Leaf Node = 1 followed by byte value.
 */
HuffmanNode* Decompressor::deserializeTree(BitReader& reader) {
    bool bit;
    if (!reader.readBit(bit)) {
        std::cerr << "Failed to read bit while deserializing tree.\n";
        return nullptr;
    }

    if (bit) {
        // Leaf node
        unsigned char byte;
        if (!reader.readByte(byte)) {
            std::cerr << "Failed to read byte for leaf node.\n";
            return nullptr;
        }
        return new HuffmanNode(byte, 0);
    }

    // Internal node
    HuffmanNode* left = deserializeTree(reader);
    HuffmanNode* right = deserializeTree(reader);

    if (!left || !right) {
        std::cerr << "Incomplete subtree during tree reconstruction.\n";
        return nullptr;
    }

    return new HuffmanNode(0, left, right);
}

/**
 * Traverses bitstream and decodes the original content using Huffman Tree.
 */
void Decompressor::decode(BitReader& reader, std::ostream& output, HuffmanNode* root, uint64_t originalSize) {
    HuffmanNode* current = root;
    bool bit;
    uint64_t bytesWritten = 0;

    while (bytesWritten < originalSize && reader.readBit(bit)) {
        current = bit ? current->right : current->left;

        // Write symbol when leaf reached
        if (current->isLeaf()) {
            output.put(current->byte);
            ++bytesWritten;
            current = root;
        }
    }

    if (bytesWritten < originalSize) {
        std::cerr << "Warning: Expected " << originalSize
                  << " bytes, but only decoded " << bytesWritten << " bytes.\n";
    }
}

/**
 * Returns the original file size read from compressed file.
 */
uint64_t Decompressor::getOriginalFileSize() const {
    return originalFileSize;
}
