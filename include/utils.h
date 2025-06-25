#ifndef UTILS_H
#define UTILS_H

#include <string>

// Utility: Compares two files byte by byte.
// Returns true if both files have identical content, false otherwise.
bool compareFiles(const std::string& file1, const std::string& file2);

#endif // UTILS_H
