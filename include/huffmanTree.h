#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include <unordered_map>
#include <queue>
#include <vector>

// Node structure used in the Huffman Tree
struct HuffmanNode {
    unsigned char byte;
    int freq;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(unsigned char b, int f) : byte(b), freq(f), left(nullptr), right(nullptr) {}
    HuffmanNode(HuffmanNode* l, HuffmanNode* r) : byte(0), freq(l->freq + r->freq), left(l), right(r) {}
};

// Comparison logic for min-heap (priority queue)
struct CompareNode {
    bool operator()(HuffmanNode* a, HuffmanNode* b) {
        return a->freq > b->freq;
    }
};

class HuffmanTree {
public:
    // Build the Huffman Tree using the given frequency map
    void build(const std::unordered_map<unsigned char, int>& freqMap);

    // Get the root of the tree
    HuffmanNode* getRoot() const;

private:
    HuffmanNode* root = nullptr;
};

#endif // HUFFMAN_TREE_H
