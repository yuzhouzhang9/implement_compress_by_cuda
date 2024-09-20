#include "../src/cpp_compression/cpp_compression.h"
#include "../src/cpp_compression/cpp_huffman_tree.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <thread>
#include <future>
#include <string>

// Function to check if a file exists
bool file_exists(const std::string& filename) {
    std::ifstream file(filename.c_str());
    return file.good();
}

std::vector<uint16_t> read_binary_file(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return {};
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint16_t> buffer(size / sizeof(uint16_t));

    if (file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        std::cout << "Successfully read " << size << " bytes from file: " << filename << std::endl;
    } else {
        std::cerr << "Failed to read file: " << filename << std::endl;
        return {};
    }

    return buffer;
}




void process_data(int file_index, std::vector<uint16_t>& data) {
    if (data.empty()) {
        std::cout <<"file_index"<<file_index<<"No data available (file may not exist or failed to read)" << std::endl;
        return;
    } 
    std::cout << "file_index"<<file_index<<"Processing data..." << std::endl;
    CustomCompressor cc = CustomCompressor();
    // compress data using Huffman encoding
    std::map<uint16_t,std::vector<bool>> huffman_encoding_method = cc.get_huffman_encoding(cc.calculate_probabilities(data));
    std::vector<bool> compressed_data = cc.compress_with_huffman(data, huffman_encoding_method);
    
    std::cout << "Compressed file " << file_index << ":" << std::endl;
    std::cout << "Compressed size: " << compressed_data.size() << " bits" << std::endl;
    
    std::vector<std::pair<uint16_t,std::vector<bool>>> huffman_encoding_method_story;
    for(auto& pair : huffman_encoding_method){
        huffman_encoding_method_story.push_back(pair);
    }
    

    // Create binary file names
    std::string compressed_file_name = "../data/compressed_data_" + std::to_string(file_index) + ".bin";
    std::string encoding_file_name = "../data/huffman_encoding_" + std::to_string(file_index) + ".bin";
    
    // Write compressed data to binary file
    std::ofstream compressed_file(compressed_file_name, std::ios::binary);
    if (compressed_file.is_open()) {
        // Convert vector<bool> to vector<char> for writing
        std::vector<char> compressed_bytes((compressed_data.size() + 7) / 8, 0);
        for (size_t i = 0; i < compressed_data.size(); ++i) {
            if (compressed_data[i]) {
                compressed_bytes[i / 8] |= (1 << (i % 8));
            }
        }
        // last byte might not be full
        uint8_t idx = compressed_data.size() % 8;
        compressed_bytes.push_back(char(idx));
        compressed_file.write(reinterpret_cast<const char*>(compressed_bytes.data()), compressed_bytes.size());
        compressed_file.close();
        std::cout << "Compressed data written to " << compressed_file_name << std::endl;
    } else {
        std::cerr << "Unable to open file_idx" + std::to_string(file_index) + " for writing compressed data." << std::endl;
    }
    
    // Write Huffman encoding to binary file
    std::ofstream encoding_file(encoding_file_name, std::ios::binary);
    if (encoding_file.is_open()) {
        // Write the size of huffman_encoding_method_story
        size_t size = huffman_encoding_method_story.size();
        encoding_file.write(reinterpret_cast<const char*>(&size), sizeof(size));
        
        // Write each pair in huffman_encoding_method_story
        for (const auto& pair : huffman_encoding_method_story) {
            encoding_file.write(reinterpret_cast<const char*>(&pair.first), sizeof(pair.first));
            
            // not over 8 bits
            uint8_t code_size = pair.second.size(); 
            encoding_file.write(reinterpret_cast<const char*>(&code_size), sizeof(code_size));

            std::vector<char> code_bytes((code_size + 7) / 8, 0);
            for (size_t i = 0; i < code_size; ++i) {
                if (pair.second[i]) {
                    code_bytes[i / 8] |= (1 << (i % 8));
                }
            }
            encoding_file.write(code_bytes.data(), code_bytes.size());
        }
        encoding_file.close();
        std::cout << "Huffman encoding written to " << encoding_file_name << std::endl;
    } else {
        std::cerr << "Unable to open file for writing Huffman encoding." << std::endl;
    }
    
    std::cout << std::endl;
}

void write_to_file_uint8(const std::string& filename, const std::vector<uint8_t>& data) {
    std::ofstream compressed_file(filename, std::ios::binary);
    if (compressed_file.is_open()) {
        // Write the size of the original data
        uint32_t original_size = static_cast<uint32_t>(data.size());
        compressed_file.write(reinterpret_cast<const char*>(&original_size), sizeof(original_size));

        // Write the data directly, as it's already in uint8_t format
        compressed_file.write(reinterpret_cast<const char*>(data.data()), data.size());

        compressed_file.close();
        std::cout << "Data written to " << filename << std::endl;
    } else {
        std::cerr << "Unable to open file " << filename << " for writing data." << std::endl;
    }
}

void process_data_in_lz77(int file_index,const std::vector<uint16_t>& data) {
    if (data.empty()) {
        std::cout <<"file_index"<<file_index<<"No data available (file may not exist or failed to read)" << std::endl;
        return;
    } 
    std::cout << "file_index"<<file_index<<"Processing data..." << std::endl;
    CustomCompressor cc = CustomCompressor();
    std::vector<uint8_t> compressed_data = cc.compress_with_lz77(data);
    std::cout << "Compressed file " << file_index << ":" << std::endl;
    std::cout << "Compressed size : " << compressed_data.size() << " bytes with lz77" << std::endl;
    // Create binary file names
    std::string compressed_file_name = "../data/compressed_data_in_lz77" + std::to_string(file_index) + ".bin";
    write_to_file_uint8(compressed_file_name,compressed_data);
}

int main() {
    std::vector<std::future<std::vector<uint16_t>>> read_futures;
    std::vector<std::future<void>> process_futures;

    // Start file reading tasks
    for (int i = 0; i < 10; ++i) {
        std::string filename = "../data/data" + std::to_string(i) + ".bin";
        if (file_exists(filename)) {
            read_futures.push_back(std::async(std::launch::async, read_binary_file, filename));
        } else {
            std::cerr << "File does not exist: " << filename << std::endl;
            read_futures.push_back(std::async(std::launch::deferred, []() { return std::vector<uint16_t>(); }));
        }
    }

    // Start data processing tasks
    for (int i = 0; i < 1; ++i) {
        process_futures.push_back(std::async(std::launch::async, [i, &read_futures]() {
            std::vector<uint16_t> data = read_futures[i].get();
            // process_data(i, data);
            std::cout<<"Processing file_index "<<i<<std::endl;
            process_data_in_lz77(i,data);
        }));
    }

    // Wait for all processing tasks to complete
    for (auto& future : process_futures) {
        future.wait();
    }

    return 0;
}