#include "huffmanTree.h"
#include <iostream>

// Build Huffman tree using priority queue
void HuffmanTree::build(const std::unordered_map<unsigned char, int>& freqMap) {
    std::priority_queue<std::shared_ptr<HuffmanNode>, std::vector<std::shared_ptr<HuffmanNode>>, Compare> pq;

    // Create leaf nodes and push into priority queue
    for (const auto& [byte, freq] : freqMap) {
        pq.push(std::make_shared<HuffmanNode>(byte, freq));
    }

    // Build tree
    while (pq.size() > 1) {
        auto left = pq.top(); pq.pop();
        auto right = pq.top(); pq.pop();
        auto parent = std::make_shared<HuffmanNode>(left->frequency + right->frequency, left, right);
        pq.push(parent);
    }

    if (!pq.empty()) {
        root = pq.top();
    }
}

// Generate Huffman codes by traversing the tree
void HuffmanTree::generateCodes() {
    codes.clear();
    if (root) {
        generateCodesHelper(root, "");
    }
}

// Recursive helper to assign codes to bytes
void HuffmanTree::generateCodesHelper(const std::shared_ptr<HuffmanNode>& node, const std::string& code) {
    if (!node) return;

    if (node->isLeaf()) {
        codes[node->byte] = code;
        return;
    }

    generateCodesHelper(node->left, code + "0");
    generateCodesHelper(node->right, code + "1");
}

// Return the generated code map
const std::unordered_map<unsigned char, std::string>& HuffmanTree::getCodes() const {
    return codes;
}
