#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include <unordered_map>
#include <queue>
#include <string>
#include <memory>

// Node structure for Huffman Tree
struct HuffmanNode {
    unsigned char byte;
    int frequency;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(unsigned char b, int freq)
        : byte(b), frequency(freq), left(nullptr), right(nullptr) {}

    HuffmanNode(int freq, HuffmanNode* l, HuffmanNode* r)
        : byte(0), frequency(freq), left(l), right(r) {}
};

// Comparator for priority queue
struct Compare {
    bool operator()(HuffmanNode* a, HuffmanNode* b) {
        return a->frequency > b->frequency;
    }
};

class HuffmanTree {
public:
    // Build Huffman Tree from frequency map
    void build(const std::unordered_map<unsigned char, int>& freqMap);

    // Get the Huffman codes map
    const std::unordered_map<unsigned char, std::string>& getHuffmanCodes() const;

private:
    HuffmanNode* root = nullptr;
    std::unordered_map<unsigned char, std::string> huffmanCodes;

    // Recursive helper to generate codes from the tree
    void generateCodes(HuffmanNode* node, const std::string& code);
};

#endif // HUFFMAN_TREE_H
