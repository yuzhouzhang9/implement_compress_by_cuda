#include "cpp_huffman_tree.h"

// Destructor: destroys the Huffman tree
HuffmanTree::~HuffmanTree() {
    delete_tree(root);
}

// Build Huffman tree using frequency table
HuffmanNode* HuffmanTree::build_tree(const std::map<uint16_t, double>& probabilities) {
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, Compare> min_heap;

    // Insert all characters and their probabilities as leaf nodes into the priority queue
    for (const auto& pair : probabilities) {
        min_heap.push(new HuffmanNode(pair.first, pair.second));
    }

    // Repeatedly extract two nodes with lowest probabilities from the priority queue and merge them
    while (min_heap.size() > 1) {
        HuffmanNode* left = min_heap.top();
        min_heap.pop();
        HuffmanNode* right = min_heap.top();
        min_heap.pop();

        // Create a new node with probability equal to the sum of its two child nodes
        HuffmanNode* new_node = new HuffmanNode(0, left->probability + right->probability);
        new_node->left = left;
        new_node->right = right;

        min_heap.push(new_node);
    }

    // The last remaining node is the root of the Huffman tree
    root = min_heap.top();

    return root;
}

// Recursively generate Huffman codes
void HuffmanTree::generate_codes_helper(HuffmanNode* node, const std::vector<bool>& code, std::map<uint16_t, std::vector<bool>>& huffman_codes) {
    if (!node) return;

    // If it's a leaf node, save its Huffman code
    if (!node->left && !node->right) {
        huffman_codes[node->value] = code;
    }

    if (node->left) {
        std::vector<bool> left_code = code;
        left_code.push_back(false);
        generate_codes_helper(node->left, left_code, huffman_codes);
    }

    if (node->right) {
        std::vector<bool> right_code = code;
        right_code.push_back(true);
        generate_codes_helper(node->right, right_code, huffman_codes);
    }
}

// Generate Huffman code table
std::map<uint16_t, std::vector<bool>> HuffmanTree::generate_codes() {
    std::map<uint16_t, std::vector<bool>> huffman_codes;
    std::vector<bool> code;
    generate_codes_helper(root, code, huffman_codes);
    return huffman_codes;
}

// Delete Huffman tree nodes
void HuffmanTree::delete_tree(HuffmanNode* node) {
    if (!node) return;
    delete_tree(node->left);
    delete_tree(node->right);
    delete node;
}


// Delete Huffman tree nodes
void HuffmanTree::delete_huffman_tree() {
    delete_tree(root);
}