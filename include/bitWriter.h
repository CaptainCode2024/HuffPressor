#ifndef BITWRITER_H
#define BITWRITER_H

#include <ostream>
#include <string>

// Forward declaration to avoid circular dependency with huffmanTree.h
class HuffmanNode;

/*
 * BitWriter is a utility class that allows writing individual bits
 * (not just full bytes) to an output stream efficiently.
 * It buffers bits and flushes them as bytes, used during compression.
 */
class BitWriter {
public:
    // Constructor: binds the writer to an output stream
    explicit BitWriter(std::ostream& outputStream);

    // Destructor: flushes any remaining bits in buffer
    ~BitWriter();

    // Writes a single bit (true for 1, false for 0)
    void writeBit(bool bit);

    // Writes a full byte (8 bits) bit-by-bit into the stream
    void writeByte(unsigned char byte);

    // Writes a sequence of bits represented as a string of '0' and '1'
    void writeBits(const std::string& bits);

    // Writes the serialized Huffman tree (pre-order format)
    void writeTree(HuffmanNode* root);

    // Flushes remaining bits (pads with 0s to complete a byte)
    void flush();

private:
    std::ostream& out;         // Output stream reference
    unsigned char buffer = 0;  // Bit buffer (8-bit accumulator)
    int bitCount = 0;          // Number of bits currently in buffer

    // Recursively serializes the Huffman tree
    void serializeTree(HuffmanNode* node);
};

#endif // BITWRITER_H
