#include "bitReader.h"
#include <iostream> // For debugging output

// Constructor: binds the BitReader to an input stream.
// Also initializes buffer and bit counter.
BitReader::BitReader(std::istream& in)
    : inputStream(in), buffer(0), bitsRemaining(0) {}

// Reads a single bit from the input stream.
// If no bits are left in the buffer, it reads a new byte into the buffer.
bool BitReader::readBit(bool& bit) {
    // Refill buffer if all bits are consumed
    if (bitsRemaining == 0) {
        char byte;
        if (!inputStream.get(byte)) {
            std::cerr << "End of stream or error while reading a byte.\n";
            return false;
        }

        buffer = static_cast<unsigned char>(byte);
        bitsRemaining = 8;

        std::cout << "Loaded byte into buffer: "
                  << std::hex << static_cast<int>(buffer)
                  << " (" << std::dec << static_cast<int>(buffer) << ")\n";
    }

    // Extract the next bit from the buffer (MSB to LSB)
    bit = (buffer >> (bitsRemaining - 1)) & 1;
    bitsRemaining--;

    std::cout << "Bit Read: " << bit
              << " | Bits Remaining: " << bitsRemaining << "\n";

    return true;
}

// Reads a full byte (8 bits) by calling readBit() 8 times.
// Assembles bits from MSB to LSB into a single byte.
bool BitReader::readByte(unsigned char& byte) {
    byte = 0;
    for (int i = 0; i < 8; ++i) {
        bool bit;
        if (!readBit(bit)) {
            std::cerr << "Failed to read bit while constructing byte.\n";
            return false;
        }
        byte = (byte << 1) | bit;
    }

    std::cout << "Full Byte Read: "
              << static_cast<int>(byte)
              << " (" << static_cast<char>(byte) << ")\n";

    return true;
}

// Skips remaining bits in the current buffer and aligns to the next full byte.
void BitReader::alignToByte() {
    if (bitsRemaining > 0) {
        std::cout << "Aligning to byte boundary. Discarding " 
                  << bitsRemaining << " remaining bits.\n";
    }
    bitsRemaining = 0;
}
