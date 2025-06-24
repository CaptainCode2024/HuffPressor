#ifndef BITWRITER_H
#define BITWRITER_H

#include <ostream>
#include <string>

class HuffmanNode; // Forward declaration

class BitWriter {
public:
    explicit BitWriter(std::ostream& outputStream);
    ~BitWriter();

    // Write bits from a string of 0s and 1s
    void writeBits(const std::string& bits);

    // Serialize Huffman tree before writing encoded data
    void writeTree(HuffmanNode* root);

    // Flush remaining bits to output stream
    void flush();

private:
    std::ostream& out;
    unsigned char buffer = 0;
    int bitCount = 0;

    // Recursive function to serialize tree
    void serializeTree(HuffmanNode* node);

    // Helper to write a single bit to the buffer
    void writeBit(bool bit);
};

#endif // BITWRITER_H
