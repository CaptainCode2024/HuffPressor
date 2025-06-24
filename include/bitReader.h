#ifndef BITREADER_H
#define BITREADER_H

#include <istream>

class BitReader {
public:
    BitReader(std::istream& input);

    // Read the next bit from stream
    bool readBit(bool& bit);

    // Read next byte (used for reading characters directly)
    bool readByte(unsigned char& byte);

private:
    std::istream& inputStream;
    unsigned char buffer;
    int bitsRemaining;
};

#endif // BITREADER_H
