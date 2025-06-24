#include "huffmanTree.h"

// Build Huffman Tree from the frequency map
void HuffmanTree::build(const std::unordered_map<unsigned char, int>& freqMap) {
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, Compare> pq;

    // Step 1: Push all frequency nodes into the priority queue
    for (const auto& [byte, freq] : freqMap) {
        pq.push(new HuffmanNode(byte, freq));
    }

    // Step 2: Combine nodes to form the Huffman Tree
    while (pq.size() > 1) {
        HuffmanNode* left = pq.top(); pq.pop();
        HuffmanNode* right = pq.top(); pq.pop();

        HuffmanNode* parent = new HuffmanNode(left->frequency + right->frequency, left, right);
        pq.push(parent);
    }

    // Step 3: The final node is the root
    root = pq.top();

    // Step 4: Generate Huffman codes from the tree
    generateCodes(root, "");
}

// Recursively generate Huffman codes from the tree
void HuffmanTree::generateCodes(HuffmanNode* node, const std::string& code) {
    if (!node) return;

    // Leaf node contains a byte
    if (!node->left && !node->right) {
        huffmanCodes[node->byte] = code;
        return;
    }

    generateCodes(node->left, code + "0");
    generateCodes(node->right, code + "1");
}

// Expose Huffman codes for use in compression
const std::unordered_map<unsigned char, std::string>& HuffmanTree::getHuffmanCodes() const {
    return huffmanCodes;
}
