#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include <map>
#include <string>
#include <queue>
#include <cstdint>

// Huffman tree node
struct HuffmanNode {
    std::uint16_t value;
    double probability;
    HuffmanNode* left;
    HuffmanNode* right;

    // Constructor
    HuffmanNode(std::uint16_t value, double w) : value(value), probability(w), left(nullptr), right(nullptr) {}
};

// Custom comparison function for priority queue
struct Compare {
    bool operator()(HuffmanNode* l, HuffmanNode* r) {
        return l->probability > r->probability;  // Lower frequency has higher priority, true means l > r
    }
};

// Huffman tree class
class HuffmanTree {
public:
    // Constructor
    HuffmanTree() : root(nullptr) {}

    // Destructor
    ~HuffmanTree();

    // Build Huffman tree using character probability table
    HuffmanNode* build_tree(const std::map<uint16_t, double>& probabilities);

    // Generate Huffman code table
    std::map<uint16_t, std::vector<bool>> generate_codes();

    void delete_huffman_tree();

private:
    HuffmanNode* root;

    // Recursively generate Huffman codes
    void generate_codes_helper(HuffmanNode* node, const std::vector<bool>& code, std::map<uint16_t, std::vector<bool>>& huffman_codes);

    // Delete Huffman tree
    void delete_tree(HuffmanNode* node);
};

#endif // HUFFMAN_TREE_H