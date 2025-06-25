#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include <unordered_map>
#include <queue>
#include <string>

struct HuffmanNode {
    unsigned char byte;
    int frequency;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(unsigned char b, int freq);
    HuffmanNode(int freq, HuffmanNode* l, HuffmanNode* r);
    ~HuffmanNode() = default;  // Explicit default destructor

    bool isLeaf() const;
};

class HuffmanTree {
public:
    HuffmanTree() = default;  // <-- Added default constructor

    void build(const std::unordered_map<unsigned char, int>& freqMap);
    const std::unordered_map<unsigned char, std::string>& getHuffmanCodes() const;
    HuffmanNode* getRoot() const;
    void generateCodes();
    ~HuffmanTree();

    HuffmanTree(const HuffmanTree&) = delete;
    HuffmanTree& operator=(const HuffmanTree&) = delete;

private:
    HuffmanNode* root = nullptr;
    std::unordered_map<unsigned char, std::string> codes;

    void generateCodes(HuffmanNode* node, const std::string& currentCode);
    void destroyTree(HuffmanNode* node);
};

#endif // HUFFMANTREE_H
