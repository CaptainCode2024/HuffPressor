#include "huffmanTree.h"
#include <queue>

// Constructor for leaf node (stores actual byte and its frequency)
HuffmanNode::HuffmanNode(unsigned char b, int freq)
    : byte(b), frequency(freq), left(nullptr), right(nullptr) {}

// Constructor for internal node (combines two children)
HuffmanNode::HuffmanNode(int freq, HuffmanNode* l, HuffmanNode* r)
    : byte(0), frequency(freq), left(l), right(r) {}

// Destructor (currently does nothing, tree cleanup is handled separately)
HuffmanNode::~HuffmanNode() {}

// Returns true if the node has no children
bool HuffmanNode::isLeaf() const {
    return !left && !right;
}

// Build the Huffman Tree from a frequency map
void HuffmanTree::build(const std::unordered_map<unsigned char, int>& freqMap) {
    // Min-heap to combine the least frequent nodes
    auto cmp = [](HuffmanNode* a, HuffmanNode* b) { return a->frequency > b->frequency; };
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, decltype(cmp)> pq(cmp);

    // Push all characters with their frequencies into the priority queue
    for (const auto& [byte, freq] : freqMap) {
        pq.push(new HuffmanNode(byte, freq));
    }

    // Merge nodes until one root remains
    while (pq.size() > 1) {
        HuffmanNode* left = pq.top(); pq.pop();
        HuffmanNode* right = pq.top(); pq.pop();
        HuffmanNode* parent = new HuffmanNode(left->frequency + right->frequency, left, right);
        pq.push(parent);
    }

    // Set root of the tree
    root = pq.empty() ? nullptr : pq.top();

    // Generate Huffman codes starting from root
    if (root) generateCodes();
}

// Recursive function to assign binary codes to each byte
void HuffmanTree::generateCodes(HuffmanNode* node, const std::string& currentCode) {
    if (!node) return;

    if (node->isLeaf()) {
        codes[node->byte] = currentCode; // Store code for leaf byte
    }

    generateCodes(node->left, currentCode + "0");
    generateCodes(node->right, currentCode + "1");
}

// Wrapper to start code generation from root
void HuffmanTree::generateCodes() {
    codes.clear(); // Clear any previous state
    generateCodes(root, "");
}

// Returns the generated Huffman codes
const std::unordered_map<unsigned char, std::string>& HuffmanTree::getHuffmanCodes() const {
    return codes;
}

// Returns the root node of the Huffman Tree
HuffmanNode* HuffmanTree::getRoot() const {
    return root;
}

// Destructor to free memory used by the tree
HuffmanTree::~HuffmanTree() {
    destroyTree(root);
}

// Helper to recursively delete nodes
void HuffmanTree::destroyTree(HuffmanNode* node) {
    if (!node) return;
    destroyTree(node->left);
    destroyTree(node->right);
    delete node;
}
