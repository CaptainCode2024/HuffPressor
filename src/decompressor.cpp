#include "decompressor.h"
#include "huffmanTree.h"
#include "config.h"

#include <fstream>
#include <iostream>
#include <cstdint>


Decompressor::~Decompressor() {
    freeTree(root);
}

void Decompressor::freeTree(HuffmanNode* node) {
    if (!node) return;
    freeTree(node->left);
    freeTree(node->right);
    delete node;
}

bool Decompressor::decompressFile(const std::string& inputFilename, const std::string& outputFilename) {
    std::ifstream input(inputFilename, std::ios::binary);
    if (!input.is_open()) {
#if ENABLE_LOGGING
        std::cerr << "Failed to open compressed input file: " << inputFilename << "\n";
#endif
        return false;
    }

    std::ofstream output(outputFilename, std::ios::binary);
    if (!output.is_open()) {
#if LOGGING_ENABLED
        std::cerr << "Failed to open output file: " << outputFilename << "\n";
#endif
        return false;
    }

    BitReader reader(input);

    // Step 1: Deserialize Huffman Tree
    root = deserializeTree(reader);
    if (!root) {
#if LOGGING_ENABLED
        std::cerr << "Tree deserialization failed. Possibly corrupted input.\n";
#endif
        return false;
    }

#if LOGGING_ENABLED
    std::cout << "Huffman Tree deserialized successfully.\n";
#endif

    // Step 2: Align to byte boundary
    reader.alignToByte();

    // Step 3: Read original file size
    uint64_t originalSize = 0;
    for (int i = 7; i >= 0; --i) {
        unsigned char sizeByte;
        if (!reader.readByte(sizeByte)) {
#if LOGGING_ENABLED
            std::cerr << "Failed to read file size metadata.\n";
#endif
            return false;
        }
        originalSize |= static_cast<uint64_t>(sizeByte) << (8 * i);
    }

    originalFileSize = originalSize;

#if LOGGING_ENABLED
    std::cout << "Original file size to decode: " << originalFileSize << " bytes\n";
#endif

    // Step 4: Decode
    decode(reader, output, root, originalFileSize);

#if LOGGING_ENABLED
    std::cout << "Decompression complete. Output saved at: " << outputFilename << "\n";
#endif
    return true;
}

HuffmanNode* Decompressor::deserializeTree(BitReader& reader) {
    bool bit;
    if (!reader.readBit(bit)) {
#if LOGGING_ENABLED
        std::cerr << "Failed to read bit while deserializing tree.\n";
#endif
        return nullptr;
    }

    if (bit) {
        unsigned char byte;
        if (!reader.readByte(byte)) {
#if LOGGING_ENABLED
            std::cerr << "Failed to read byte for leaf node.\n";
#endif
            return nullptr;
        }
        return new HuffmanNode(byte, 0);
    }

    HuffmanNode* left = deserializeTree(reader);
    HuffmanNode* right = deserializeTree(reader);

    if (!left || !right) {
#if LOGGING_ENABLED
        std::cerr << "Incomplete subtree during tree reconstruction.\n";
#endif
        return nullptr;
    }

    return new HuffmanNode(0, left, right);
}

void Decompressor::decode(BitReader& reader, std::ostream& output, HuffmanNode* root, uint64_t originalSize) {
    HuffmanNode* current = root;
    bool bit;
    uint64_t bytesWritten = 0;

    while (bytesWritten < originalSize && reader.readBit(bit)) {
        current = bit ? current->right : current->left;

        if (current->isLeaf()) {
            output.put(current->byte);
            ++bytesWritten;
            current = root;
        }
    }

    if (bytesWritten < originalSize) {
#if LOGGING_ENABLED
        std::cerr << "Warning: Expected " << originalSize
                  << " bytes, but only decoded " << bytesWritten << " bytes.\n";
#endif
    }
}

uint64_t Decompressor::getOriginalFileSize() const {
    return originalFileSize;
}
