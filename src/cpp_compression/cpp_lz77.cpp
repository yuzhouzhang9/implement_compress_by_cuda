#include "cpp_lz77.h"
#include <algorithm>
#include <stdexcept>

Lz77Compressor::Lz77Compressor(int window_size, int lookahead_buffer_size)
    : window_size(window_size), lookahead_buffer_size(lookahead_buffer_size) {}

Lz77Compressor::Match Lz77Compressor::find_longest_match(const std::vector<uint16_t>& data, int current_position) {
    int end_of_buffer = std::min(current_position + lookahead_buffer_size, static_cast<int>(data.size()));
    int start_of_window = std::max(0, current_position - window_size);
    Match best_match = {0, 0};

    for (int i = start_of_window; i < current_position; ++i) {
        int match_length = 0;
        while (current_position + match_length < end_of_buffer &&
               data[i + match_length] == data[current_position + match_length]) {
            ++match_length;
        }
        if (match_length > best_match.length) {
            best_match.distance = current_position - i;
            best_match.length = match_length;
        }
    }
    return best_match;
}

void Lz77Compressor::write_bits(std::vector<uint8_t>& output, uint32_t value, int bit_count) {
    static int bit_position = 0;
    while (bit_count > 0) {
        if (bit_position == 0 && bit_count >= 8) {
            output.push_back(static_cast<uint8_t>(value >> (bit_count - 8)));
            bit_count -= 8;
            value &= (1 << bit_count) - 1;
        } else {
            if (bit_position == 0) {
                output.push_back(0);
            }
            int bits_to_write = std::min(8 - bit_position, bit_count);
            output.back() |= (value >> (bit_count - bits_to_write)) << (8 - bit_position - bits_to_write);
            bit_position = (bit_position + bits_to_write) % 8;
            bit_count -= bits_to_write;
            value &= (1 << bit_count) - 1;
        }
    }
}

uint32_t Lz77Compressor::read_bits(const std::vector<uint8_t>& input, int& bit_position, int bit_count) {
    uint32_t result = 0;
    int bits_read = 0;
    while (bits_read < bit_count) {
        if (bit_position / 8 >= input.size()) {
            throw std::runtime_error("Unexpected end of input");
        }
        int bits_available = 8 - (bit_position % 8);
        int bits_to_read = std::min(bits_available, bit_count - bits_read);
        result <<= bits_to_read;
        result |= (input[bit_position / 8] >> (bits_available - bits_to_read)) & ((1 << bits_to_read) - 1);
        bit_position += bits_to_read;
        bits_read += bits_to_read;
    }
    return result;
}

std::vector<uint8_t> Lz77Compressor::compress(const std::vector<uint16_t>& data) {
    std::vector<uint8_t> compressed_data;
    int i = 0;
    while (i < static_cast<int>(data.size())) {
        Match match = find_longest_match(data, i);
        if (match.length > 2) {
            write_bits(compressed_data, 1, 1);  // Mark as a match
            write_bits(compressed_data, match.distance, 12);
            write_bits(compressed_data, match.length, 4);
            i += match.length;
        } else {
            write_bits(compressed_data, 0, 1);  // Mark as a literal
            write_bits(compressed_data, static_cast<uint16_t>(data[i]), 16);  // Use 16 bits to store int16_t
            ++i;
        }
    }
    return compressed_data;
}

std::vector<uint16_t> Lz77Compressor::decompress(const std::vector<uint8_t>& compressed_data) {
    std::vector<uint16_t> decompressed_data;
    int bit_position = 0;
    while (bit_position / 8 < compressed_data.size()) {
        if (read_bits(compressed_data, bit_position, 1) == 1) {
            uint16_t distance = read_bits(compressed_data, bit_position, 12);
            uint8_t length = read_bits(compressed_data, bit_position, 4);
            int start = decompressed_data.size() - distance;
            for (int i = 0; i < length; ++i) {
                decompressed_data.push_back(decompressed_data[start + i]);
            }
        } else {
            int16_t value = static_cast<int16_t>(read_bits(compressed_data, bit_position, 16));
            decompressed_data.push_back(value);
        }
    }
    return decompressed_data;
}