#ifndef CUSTOM_COMPRESSOR_H
#define CUSTOM_COMPRESSOR_H
#include "cpp_huffman_tree.h"
#include "cpp_lz77.h"
#include <bits/stdint-uintn.h>
#include <vector>
#include <string>
#include <cstdint>
#include <map>
class CustomCompressor {
public:
    CustomCompressor();
    ~CustomCompressor();

    // compress data
    std::vector<uint8_t> compress(const std::vector<uint16_t>& data);

    // calculate probabilities
    std::map<uint16_t,double>calculate_probabilities(const std::vector<uint16_t>& data);
    
    // get huffman encoding
    std::map<uint16_t, std::vector<bool>> get_huffman_encoding(const std::map<uint16_t,double>) const;


    // decompress data
    std::vector<uint8_t> decompress(const std::vector<uint8_t>& compressedData);

    // quant data
    std::vector<uint8_t> quant_data(const std::vector<uint8_t>& data);

    // 
    std::string get_name() const;
    
    // compres with huffman
    std::vector<bool>compress_with_huffman(const std::vector<std::uint16_t>& data,const std::map<std::uint16_t,std::vector<bool>> & endcoding);

    // decompres with huffman
    std::vector<uint16_t>decompress_with_huffman(const std::vector<bool>& data,const std::map<std::uint16_t,std::vector<bool>>& endcoding);

    std::vector<uint8_t> compress_with_lz77(const std::vector<uint16_t>& data);

    std::vector<uint16_t> decompress_with_lz77(const std::vector<uint8_t>& compressedData);

    // set compression level
    void set_compression_level(int level);

private:
    int compressionLevel;
};

#endif // CUSTOM_COMPRESSOR_H
