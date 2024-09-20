#ifndef CPP_LZ77_H
#define CPP_LZ77_H

#include <vector>
#include <cstdint>

class Lz77Compressor {
public:
    Lz77Compressor(int window_size = 4096, int lookahead_buffer_size = 16);
    std::vector<uint8_t> compress(const std::vector<uint16_t>& data);
    std::vector<uint16_t> decompress(const std::vector<uint8_t>& compressed_data);

private:
    int window_size;
    int lookahead_buffer_size;
    struct Match {
        uint16_t distance;
        uint8_t length;
    };
    Match find_longest_match(const std::vector<uint16_t>& data, int current_position);
    void write_bits(std::vector<uint8_t>& output, uint32_t value, int bit_count);
    uint32_t read_bits(const std::vector<uint8_t>& input, int& bit_position, int bit_count);
};

#endif // CPP_LZ77_H