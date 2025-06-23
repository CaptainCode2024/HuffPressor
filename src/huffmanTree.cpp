#include "huffmanTree.h"

// Builds the Huffman tree from the frequency map
void HuffmanTree::build(const std::unordered_map<unsigned char, int>& freqMap) {
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, CompareNode> minHeap;

    // Step 1: Create a leaf node for each byte and add it to the min-heap
    for (const auto& [byte, freq] : freqMap) {
        minHeap.push(new HuffmanNode(byte, freq));
    }

    // Step 2: Build the tree by combining two smallest nodes until one root remains
    while (minHeap.size() > 1) {
        HuffmanNode* left = minHeap.top(); minHeap.pop();
        HuffmanNode* right = minHeap.top(); minHeap.pop();

        HuffmanNode* merged = new HuffmanNode(left, right);
        minHeap.push(merged);
    }

    // Final remaining node is the root of the tree
    if (!minHeap.empty()) {
        root = minHeap.top();
    }
}

// Returns the root of the Huffman tree
HuffmanNode* HuffmanTree::getRoot() const {
    return root;
}
