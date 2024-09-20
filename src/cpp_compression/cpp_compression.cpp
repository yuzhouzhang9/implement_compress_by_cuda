#include "cpp_compression.h"
#include "cpp_huffman_tree.h"
#include <algorithm>
#include <bits/stdint-uintn.h>
#include <stdexcept>
#include <queue>
#include <vector>
using namespace std;
CustomCompressor::CustomCompressor() : compressionLevel(5) {}

CustomCompressor::~CustomCompressor() {}

// compress used huffman encode 
std::vector<bool> CustomCompressor::compress_with_huffman(const std::vector<std::uint16_t>& data, const std::map<std::uint16_t, std::vector<bool>>& encoding) {
    std::vector<bool> res;
    // Implement compression with Huffman encoding
    for (const uint16_t x : data) {
        auto it = encoding.find(x);
        if (it != encoding.end()) {
            res.insert(res.end(), it->second.begin(), it->second.end());
        }
    }
    return res;
}

std::vector<uint16_t> CustomCompressor::decompress_with_huffman(const std::vector<bool>& data, const std::map<std::uint16_t, std::vector<bool>>& encoding) {
    std::vector<uint16_t> decompressed;
    std::map<std::vector<bool>, uint16_t> decoding;

    // Create a decoding map from the encoding map
    for (const auto& pair : encoding) {
        decoding[pair.second] = pair.first;
    }

    std::vector<bool> current_code;
    for (const auto& bit : data) {
        current_code.push_back(bit);
        auto it = decoding.find(current_code);
        if (it != decoding.end()) {
            decompressed.push_back(it->second);
            current_code.clear();
        }
    }

    return decompressed;
}


std::vector<uint8_t> CustomCompressor::compress(const std::vector<uint16_t>& data) {
    // Implement compression logic
    // This is a simple example, actual implementation may be more complex
    std::vector<uint8_t> compressed;
    for (const auto& value : data) {
        compressed.push_back(value & 0xFF);
        compressed.push_back((value >> 8) & 0xFF);
    }
    return compressed;
}

std::map<uint16_t, double> CustomCompressor::calculate_probabilities(const std::vector<uint16_t>& data) {
    // Calculate probabilities of each value
    uint64_t data_sz = data.size();
    std::map<uint16_t, double> probabilities;
    for (const auto& value : data) {
        probabilities[value] += 1.0/data_sz;
    }
    return probabilities;
}

// get huffman encoding
std::map<uint16_t, std::vector<bool>> CustomCompressor::get_huffman_encoding(const std::map<uint16_t, double> probabilities) const {

    // Build Huffman tree
    HuffmanTree huffmanTree = HuffmanTree();// Create Huffman tree object
    huffmanTree.build_tree(probabilities);

    // Implement Huffman encoding logic
    std::map<uint16_t, std::vector<bool>> encoding;
    encoding = huffmanTree.generate_codes();
    huffmanTree.delete_huffman_tree();  // Delete Huffman tree to save memory
    return encoding;

}

// 
std::vector<uint8_t> CustomCompressor::decompress(const std::vector<uint8_t>& compressedData) {
    // Implement decompression logic
    std::vector<uint16_t> decompressed;
    for (size_t i = 0; i < compressedData.size(); i += 2) {
        if (i + 1 < compressedData.size()) {
            uint16_t value = compressedData[i] | (compressedData[i + 1] << 8);
            decompressed.push_back(value);
        }
    }
    return std::vector<uint8_t>(reinterpret_cast<uint8_t*>(decompressed.data()),
                                reinterpret_cast<uint8_t*>(decompressed.data() + decompressed.size()));
}

std::vector<uint8_t> CustomCompressor::quant_data(const std::vector<uint8_t>& data) {
    // Implement data quantization logic
    // This is a simple example, actual implementation may be more complex
    std::vector<uint8_t> quantized;
    for (const auto& value : data) {
        quantized.push_back(value & 0xF0);  // Simple 4-bit quantization
    }
    return quantized;
}

std::string CustomCompressor::get_name() const {
    return "CustomCompressor";
}

void CustomCompressor::set_compression_level(int level) {
    if (level < 1 || level > 9) {
        throw std::invalid_argument("Compression level must be between 1 and 9");
    }
    compressionLevel = level;
}