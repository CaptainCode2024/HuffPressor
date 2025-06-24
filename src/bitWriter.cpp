#include "bitWriter.h"
#include "huffmanTree.h"

BitWriter::BitWriter(std::ostream& outputStream) : out(outputStream) {}

BitWriter::~BitWriter() {
    flush(); // Ensure no bits are left unwritten
}

void BitWriter::writeBit(bool bit) {
    buffer = (buffer << 1) | bit;
    bitCount++;
    if (bitCount == 8) {
        out.put(buffer);
        bitCount = 0;
        buffer = 0;
    }
}

void BitWriter::writeBits(const std::string& bits) {
    for (char c : bits) {
        writeBit(c == '1');
    }
}

void BitWriter::flush() {
    if (bitCount > 0) {
        buffer <<= (8 - bitCount); // Pad remaining bits with 0
        out.put(buffer);
        buffer = 0;
        bitCount = 0;
    }
}

void BitWriter::writeTree(HuffmanNode* root) {
    serializeTree(root);
}

void BitWriter::serializeTree(HuffmanNode* node) {
    if (!node) return;

    if (node->isLeaf()) {
        writeBit(1); // Marker for leaf
        for (int i = 7; i >= 0; --i) {
            writeBit((node->byte >> i) & 1); // Write byte (8 bits)
        }
    } else {
        writeBit(0); // Marker for internal node
        serializeTree(node->left);
        serializeTree(node->right);
    }
}
