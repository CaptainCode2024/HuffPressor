#include "huffmanTree.h"
#include <queue>

// Constructor for leaf node
HuffmanNode::HuffmanNode(unsigned char b, int freq)
    : byte(b), frequency(freq), left(nullptr), right(nullptr) {}

// Constructor for internal node
HuffmanNode::HuffmanNode(int freq, HuffmanNode* l, HuffmanNode* r)
    : byte(0), frequency(freq), left(l), right(r) {}

HuffmanNode::~HuffmanNode() {
    // Destructor intentionally left empty
}

bool HuffmanNode::isLeaf() const {
    return !left && !right;
}

void HuffmanTree::build(const std::unordered_map<unsigned char, int>& freqMap) {
    // Min-heap to pick the two lowest-frequency nodes
    auto cmp = [](HuffmanNode* a, HuffmanNode* b) { return a->frequency > b->frequency; };
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, decltype(cmp)> pq(cmp);

    // Initialize heap with all byte frequencies
    for (const auto& [byte, freq] : freqMap) {
        pq.push(new HuffmanNode(byte, freq));
    }

    // Build tree until only one node (the root) remains
    while (pq.size() > 1) {
        HuffmanNode* left = pq.top(); pq.pop();
        HuffmanNode* right = pq.top(); pq.pop();
        HuffmanNode* parent = new HuffmanNode(left->frequency + right->frequency, left, right);
        pq.push(parent);
    }

    root = pq.empty() ? nullptr : pq.top();

    // Generate binary codes from the built tree
    if (root) generateCodes(root, "");
}

void HuffmanTree::generateCodes(HuffmanNode* node, const std::string& currentCode) {
    if (!node) return;

    if (node->isLeaf()) {
        codes[node->byte] = currentCode; // Assign code to byte
    }

    generateCodes(node->left, currentCode + "0");
    generateCodes(node->right, currentCode + "1");
}

const std::unordered_map<unsigned char, std::string>& HuffmanTree::getHuffmanCodes() const {
    return codes;
}

HuffmanNode* HuffmanTree::getRoot() const {
    return root;
}

HuffmanTree::~HuffmanTree() {
    destroyTree(root); // Clean up heap memory
}

void HuffmanTree::destroyTree(HuffmanNode* node) {
    if (!node) return;
    destroyTree(node->left);
    destroyTree(node->right);
    delete node;
}
