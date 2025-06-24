#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include <unordered_map>
#include <queue>
#include <string>

// Node structure used in the Huffman tree
struct HuffmanNode {
    unsigned char byte;       // The byte represented by this node (valid only for leaves)
    int frequency;            // Frequency of the byte
    HuffmanNode* left;        // Left child
    HuffmanNode* right;       // Right child

    HuffmanNode(unsigned char b, int freq);
    HuffmanNode(int freq, HuffmanNode* l, HuffmanNode* r);
    ~HuffmanNode();

    // Utility: check if the node is a leaf
    bool isLeaf() const;
};

class HuffmanTree {
public:
    // Build tree using a byte-frequency map
    void build(const std::unordered_map<unsigned char, int>& freqMap);

    // Returns generated Huffman codes for all bytes
    const std::unordered_map<unsigned char, std::string>& getHuffmanCodes() const;

    // Get the root node (needed for tree serialization)
    HuffmanNode* getRoot() const;

    // Public method to generate Huffman codes from the root
    void generateCodes();

    // Destructor: frees memory
    ~HuffmanTree();

private:
    HuffmanNode* root = nullptr;
    std::unordered_map<unsigned char, std::string> codes;

    // Recursively generate binary codes for each leaf
    void generateCodes(HuffmanNode* node, const std::string& currentCode);

    // Helper function to free memory recursively
    void destroyTree(HuffmanNode* node);
};

#endif // HUFFMANTREE_H
