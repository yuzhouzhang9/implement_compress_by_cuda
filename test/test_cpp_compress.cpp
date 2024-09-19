// Load data files from the 'data' directory
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <opencv2/opencv.hpp>

namespace fs = std::filesystem;

std::vector<cv::Mat> load_npy_data(const std::string& directory_path) {
    std::vector<cv::Mat> data;
    for (const auto& entry : fs::directory_iterator(directory_path)) {
        if (entry.path().extension() == ".npy") {
            cv::Mat mat = cv::imread(entry.path().string(), cv::IMREAD_UNCHANGED);
            data.push_back(mat);
        }
    }
    return data;
}

int main() {
    std::string data_directory = "data";
    std::vector<cv::Mat> loaded_data = load_npy_data(data_directory);

    // Process the loaded data here
    // ...

    return 0;
}