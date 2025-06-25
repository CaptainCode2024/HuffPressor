#include "bitReader.h"
#include <iostream> // For debug output
#include "config.h"


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
#if ENABLE_LOGGING
            std::cerr << "End of stream or error while reading a byte.\n";
#endif
            return false;
        }

        buffer = static_cast<unsigned char>(byte);
        bitsRemaining = 8;

#if ENABLE_LOGGING
        std::cout << "Loaded byte into buffer: "
                  << std::hex << static_cast<int>(buffer)
                  << " (" << std::dec << static_cast<int>(buffer) << ")\n";
#endif
    }

    // Extract the next bit from the buffer (MSB to LSB)
    bit = (buffer >> (bitsRemaining - 1)) & 1;
    bitsRemaining--;

#if ENABLE_LOGGING
    std::cout << "Bit Read: " << bit
              << " | Bits Remaining: " << bitsRemaining << "\n";
#endif

    return true;
}

// Reads a full byte (8 bits) by calling readBit() 8 times.
// Assembles bits from MSB to LSB into a single byte.
bool BitReader::readByte(unsigned char& byte) {
    byte = 0;
    for (int i = 0; i < 8; ++i) {
        bool bit;
        if (!readBit(bit)) {
#if ENABLE_LOGGING
            std::cerr << "Failed to read bit while constructing byte.\n";
#endif
            return false;
        }
        byte = (byte << 1) | bit;
    }

#if ENABLE_LOGGING
    std::cout << "Full Byte Read: "
              << static_cast<int>(byte)
              << " (" << static_cast<char>(byte) << ")\n";
#endif

    return true;
}

// Skips remaining bits in the current buffer and aligns to the next full byte.
void BitReader::alignToByte() {
    if (bitsRemaining > 0) {
#if ENABLE_LOGGING
        std::cout << "Aligning to byte boundary. Discarding "
                  << bitsRemaining << " remaining bits.\n";
#endif
        bitsRemaining = 0;
    }
}
