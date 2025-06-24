#include "bitReader.h"

BitReader::BitReader(std::istream& input)
    : inputStream(input), buffer(0), bitsRemaining(0) {}

// Reads next bit (returns false on failure)
bool BitReader::readBit(bool& bit) {
    if (bitsRemaining == 0) {
        char byte;
        if (!inputStream.get(byte)) return false;
        buffer = static_cast<unsigned char>(byte);
        bitsRemaining = 8;
    }

    bit = (buffer >> (bitsRemaining - 1)) & 1;
    bitsRemaining--;
    return true;
}

// Reads full byte (used for characters and marker)
bool BitReader::readByte(unsigned char& byte) {
    return static_cast<bool>(inputStream.read(reinterpret_cast<char*>(&byte), 1));
}
