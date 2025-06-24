#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include <memory>
#include <queue>
#include <unordered_map>
#include <string>

// Represents a node in the Huffman Tree
struct HuffmanNode {
    unsigned char byte;
    int frequency;
    std::shared_ptr<HuffmanNode> left;
    std::shared_ptr<HuffmanNode> right;

    HuffmanNode(unsigned char b, int f)
        : byte(b), frequency(f), left(nullptr), right(nullptr) {}

    HuffmanNode(int f, std::shared_ptr<HuffmanNode> l, std::shared_ptr<HuffmanNode> r)
        : byte(0), frequency(f), left(l), right(r) {}

    bool isLeaf() const {
        return !left && !right;
    }
};

// Comparator for priority queue (min-heap based on frequency)
struct Compare {
    bool operator()(const std::shared_ptr<HuffmanNode>& a, const std::shared_ptr<HuffmanNode>& b) const {
        return a->frequency > b->frequency;
    }
};

// Manages Huffman tree construction and code generation
class HuffmanTree {
public:
    void build(const std::unordered_map<unsigned char, int>& freqMap);
    void generateCodes();
    const std::unordered_map<unsigned char, std::string>& getCodes() const;

private:
    std::shared_ptr<HuffmanNode> root;
    std::unordered_map<unsigned char, std::string> codes;

    void generateCodesHelper(const std::shared_ptr<HuffmanNode>& node, const std::string& code);
};

#endif // HUFFMANTREE_H
